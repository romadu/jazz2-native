﻿#include "JJ2Tileset.h"
#include "JJ2Anims.h"

#include "../../nCine/IO/CompressionUtils.h"
#include "../../nCine/IO/GrowableMemoryFile.h"

namespace Jazz2::Compatibility
{
	void JJ2Tileset::Open(const StringView& path, bool strictParser)
	{
		auto s = fs::Open(path, FileAccessMode::Read);
		ASSERT_MSG(s->IsOpened(), "Cannot open file for reading");

		// Skip copyright notice
		s->Seek(180, SeekOrigin::Current);

		JJ2Block headerBlock(s, 262 - 180);

		uint32_t magic = headerBlock.ReadUInt32();
		ASSERT_MSG(magic == 0x454C4954 /*TILE*/, "Invalid magic string");

		uint32_t signature = headerBlock.ReadUInt32();
		ASSERT_MSG(signature == 0xAFBEADDE, "Invalid signature");

		_name = headerBlock.ReadString(32, true);

		uint16_t versionNum = headerBlock.ReadUInt16();
		_version = (versionNum <= 512 ? JJ2Version::BaseGame : JJ2Version::TSF);

		int recordedSize = headerBlock.ReadInt32();
		ASSERT_MSG(!strictParser || s->GetSize() == recordedSize, "Unexpected file size");

		// Get the CRC; would check here if it matches if we knew what variant it is AND what it applies to
		// Test file across all CRC32 variants + Adler had no matches to the value obtained from the file
		// so either the variant is something else or the CRC is not applied to the whole file but on a part
		/*int recordedCRC =*/ headerBlock.ReadInt32();

		// Read the lengths, uncompress the blocks and bail if any block could not be uncompressed
		// This could look better without all the copy-paste, but meh.
		int infoBlockPackedSize = headerBlock.ReadInt32();
		int infoBlockUnpackedSize = headerBlock.ReadInt32();
		int imageBlockPackedSize = headerBlock.ReadInt32();
		int imageBlockUnpackedSize = headerBlock.ReadInt32();
		int alphaBlockPackedSize = headerBlock.ReadInt32();
		int alphaBlockUnpackedSize = headerBlock.ReadInt32();
		int maskBlockPackedSize = headerBlock.ReadInt32();
		int maskBlockUnpackedSize = headerBlock.ReadInt32();

		JJ2Block infoBlock(s, infoBlockPackedSize, infoBlockUnpackedSize);
		JJ2Block imageBlock(s, imageBlockPackedSize, imageBlockUnpackedSize);
		JJ2Block alphaBlock(s, alphaBlockPackedSize, alphaBlockUnpackedSize);
		JJ2Block maskBlock(s, maskBlockPackedSize, maskBlockUnpackedSize);

		LoadMetadata(infoBlock);
		LoadImageData(imageBlock, alphaBlock);
		LoadMaskData(maskBlock);
	}

	void JJ2Tileset::LoadMetadata(JJ2Block& block)
	{
		for (int i = 0; i < 256; i++) {
			uint32_t color = block.ReadUInt32();
			color = (color & 0x00ffffff) | ((255 - ((color >> 24) & 0xff)) << 24);
			_palette[i] = color;
		}

		_tileCount = block.ReadInt32();

		// TODO: Use _tileCount instead of maxTiles ???
		int maxTiles = GetMaxSupportedTiles();
		_tiles = std::make_unique<TilesetTileSection[]>(maxTiles);

		for (int i = 0; i < maxTiles; ++i) {
			_tiles[i].Opaque = block.ReadBool();
		}

		// Block of unknown values, skip
		block.DiscardBytes(maxTiles);

		for (int i = 0; i < maxTiles; ++i) {
			_tiles[i].ImageDataOffset = block.ReadUInt32();
		}

		// Block of unknown values, skip
		block.DiscardBytes(4 * maxTiles);

		for (int i = 0; i < maxTiles; ++i) {
			_tiles[i].AlphaDataOffset = block.ReadUInt32();
		}

		// Block of unknown values, skip
		block.DiscardBytes(4 * maxTiles);

		for (int i = 0; i < maxTiles; ++i) {
			_tiles[i].MaskDataOffset = block.ReadUInt32();
		}

		// We don't care about the flipped masks, those are generated on runtime
		block.DiscardBytes(4 * maxTiles);
	}

	void JJ2Tileset::LoadImageData(JJ2Block& imageBlock, JJ2Block& alphaBlock)
	{
		uint8_t imageBuffer[BlockSize * BlockSize];
		uint8_t maskBuffer[BlockSize * BlockSize / 8];

		int maxTiles = GetMaxSupportedTiles();
		for (int i = 0; i < maxTiles; i++) {
			auto& tile = _tiles[i];

			imageBlock.SeekTo(tile.ImageDataOffset);
			imageBlock.ReadRawBytes(imageBuffer, sizeof(imageBuffer));
			alphaBlock.SeekTo(tile.AlphaDataOffset);
			alphaBlock.ReadRawBytes(maskBuffer, sizeof(maskBuffer));

			for (int j = 0; j < (BlockSize * BlockSize); j++) {
				uint8_t idx = imageBuffer[j];
				if (((maskBuffer[j / 8] >> (j % 8)) & 0x01) == 0x00) {
					// Empty mask
					idx = 0;
				}

				tile.Image[j] = idx;
			}
		}
	}

	void JJ2Tileset::LoadMaskData(JJ2Block& block)
	{
		//uint8_t maskBuffer[BlockSize * BlockSize / 8];

		int maxTiles = GetMaxSupportedTiles();
		for (int i = 0; i < maxTiles; i++) {
			auto& tile = _tiles[i];

			block.SeekTo(tile.MaskDataOffset);
			/*block.ReadRawBytes(maskBuffer, sizeof(maskBuffer));

			for (int j = 0; j < 128; j++) {
				uint8_t idx = maskBuffer[j];
				for (int k = 0; k < 8; k++) {
					int pixelIdx = 8 * j + k;
					int x = pixelIdx % BlockSize;
					int y = pixelIdx / BlockSize;
					if (((idx >> k) & 0x01) == 0) {
						tile.Mask[y * BlockSize + x] = 0;
					} else {
						tile.Mask[y * BlockSize + x] = 1;
					}
				}
			}*/

			block.ReadRawBytes(tile.Mask, sizeof(tile.Mask));
		}
	}

	void JJ2Tileset::Convert(const String& targetPath) const
	{
		// Rearrange tiles from '10 tiles per row' to '30 tiles per row'
		constexpr int TilesPerRow = 30;

		int width = TilesPerRow * BlockSize;
		int height = ((_tileCount - 1) / TilesPerRow + 1) * BlockSize;

		auto so = fs::Open(targetPath, FileAccessMode::Write);
		ASSERT_MSG(so->IsOpened(), "Cannot open file for writing");

		constexpr uint8_t flags = 0x20 | 0x40; // Mask and palette included

		so->WriteValue<uint64_t>(0xB8EF8498E2BFBBEF);
		so->WriteValue<uint32_t>(0x0002208F | (flags << 24)); // Version 2 is reserved for sprites (or bigger images)

		// TODO: Use single channel instead
		so->WriteValue<uint8_t>(4);
		so->WriteValue<uint32_t>(width);
		so->WriteValue<uint32_t>(height);

		GrowableMemoryFile co(1024 * 1024);

		// Palette
		co.Write(_palette, sizeof(_palette));

		// Mask
		co.WriteValue<uint32_t>(_tileCount * sizeof(_tiles[0].Mask));
		for (int i = 0; i < _tileCount; i++) {
			auto& tile = _tiles[i];
			co.Write(tile.Mask, sizeof(tile.Mask));
		}

		// Compress palette and mask
		int32_t compressedSize = CompressionUtils::GetMaxDeflatedSize(co.GetSize());
		std::unique_ptr<uint8_t[]> compressedBuffer = std::make_unique<uint8_t[]>(compressedSize);
		compressedSize = CompressionUtils::Deflate(co.GetBuffer(), co.GetSize(), compressedBuffer.get(), compressedSize);
		ASSERT(compressedSize > 0);

		so->WriteValue<int32_t>(compressedSize);
		so->WriteValue<int32_t>(co.GetSize());
		so->Write(compressedBuffer.get(), compressedSize);

		// Image
		std::unique_ptr<uint8_t[]> pixels = std::make_unique<uint8_t[]>(width * height * 4);

		for (int i = 0; i < _tileCount; i++) {
			auto& tile = _tiles[i];

			int ox = (i % TilesPerRow) * BlockSize;
			int oy = (i / TilesPerRow) * BlockSize;
			for (int y = 0; y < BlockSize; y++) {
				for (int x = 0; x < BlockSize; x++) {
					auto& src = tile.Image[y * BlockSize + x];

					pixels[(width * (oy + y) + ox + x) * 4] = src;
					pixels[(width * (oy + y) + ox + x) * 4 + 1] = src;
					pixels[(width * (oy + y) + ox + x) * 4 + 2] = src;
					pixels[(width * (oy + y) + ox + x) * 4 + 3] = (src == 0 ? 0 : 255);
				}
			}
		}

		// TODO: Use single channel instead
		JJ2Anims::WriteImageToFileInternal(so, pixels.get(), width, height, 4);
	}
}