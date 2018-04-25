/*
 @ 0xCCCCCCCC
*/

using System;
using System.Collections.Generic;
using System.IO;

namespace SequencedImagePacker
{
    class ChunkInfo
    {
        public ChunkInfo(uint id, uint offset)
        {
            Id = id;
            OffsetInPackage = offset;
        }

        public uint Id
        {
            get;
        }

        public uint OffsetInPackage
        {
            get;set;
        }
    }

    class ImagePackage
    {
        private readonly List<ImageChunk> _chunks = new List<ImageChunk>();

        public ImagePackage(IEnumerable<string> collections)
        {
            foreach (var collection in collections)
            {
                _chunks.Add(ImageChunk.FromCollection(collection));
            }
        }

        public byte[] Marshal()
        {
            var chunkDataList = new List<byte[]>(_chunks.Count);
            foreach (var chunk in _chunks)
            {
                chunkDataList.Add(chunk.Marshal());
            }

            var infos = GenerateChunkInfos(chunkDataList);

            if (infos.Count != chunkDataList.Count)
            {
                throw new Exception("Chunk info must match with chunk data!");
            }

            var mem = new MemoryStream();
            using (var writer = new BinaryWriter(mem))
            {
                // Number of chunks.
                writer.Write((uint)infos.Count);

                // Chunk info header
                foreach (var info in infos)
                {
                    writer.Write(info.Id);
                    writer.Write(info.OffsetInPackage);
                }

                // Chunk data
                foreach (var chunkData in chunkDataList)
                {
                    writer.Write(chunkData);
                }
            }

            return mem.ToArray();
        }

        private static List<ChunkInfo> GenerateChunkInfos(List<byte[]> dataChain)
        {
            var infos = new List<ChunkInfo> {new ChunkInfo(0, 0)};

            for (int i = 0; i < dataChain.Count - 1; ++i)
            {
                var newOffset = infos[i].OffsetInPackage + (uint)dataChain[i].Length;
                infos.Add(new ChunkInfo((uint)i+1, newOffset));
            }

            var headerSizeInBytes = (uint)(1 + infos.Count * 2) * 4;

            Console.WriteLine("Header size in bytes: {0}", headerSizeInBytes);

            foreach (var info in infos)
            {
                info.OffsetInPackage += headerSizeInBytes;
            }

            return infos;
        }
    }
}
