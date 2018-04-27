/*
 @ 0xCCCCCCCC
*/

using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace SequencedImagePacker
{
    class ImageInfo
    {
        public ImageInfo(uint offset, uint size)
        {
            StartOffsetInChunk = offset;
            Size = size;
        }

        public uint StartOffsetInChunk
        {
            get;set;
        }

        public uint Size
        {
            get;
        }
    }

    class ImageChunk
    {
        private readonly List<ImageInfo> _imageInfos = new List<ImageInfo>();
        private readonly MemoryStream _chunkData = new MemoryStream();

        public static ImageChunk FromCollection(string dir)
        {
            var chunk = new ImageChunk();

            Console.Write("Consolidating {0}\n", dir);

            var images = Directory.EnumerateFiles(dir).ToList();
            foreach (var imgPath in images)
            {
                Console.Write("\tProcessing {0}\n", Path.GetFileName(imgPath));
                var data = File.ReadAllBytes(imgPath);
                chunk.Append(data);
            }

            return chunk;
        }

        public void Append(byte[] rawBytes)
        {
            var offset = Convert.ToUInt32(_chunkData.Length);
            var size = Convert.ToUInt32(rawBytes.Length);

            _imageInfos.Add(new ImageInfo(offset, size));
            _chunkData.Write(rawBytes, 0, rawBytes.Length);
        }

        public byte[] Marshal()
        {
            // Fix in-chunk offset now that we know the size of chunk header.
            var chunkHeaderSize = (uint)(1 + _imageInfos.Count * 2) * 4;
            Console.WriteLine("\tChunk size: {0}", chunkHeaderSize);

            foreach (var info in _imageInfos)
            {
                info.StartOffsetInChunk += chunkHeaderSize;
            }

            var mem = new MemoryStream();
            using (var writer = new BinaryWriter(mem))
            {
                writer.Write(Convert.ToUInt32(_imageInfos.Count));

                foreach (var info in _imageInfos)
                {
                    writer.Write(info.StartOffsetInChunk);
                    writer.Write(info.Size);
                }

                writer.Write(_chunkData.GetBuffer());
            }

            return mem.ToArray();
        }
    }
}
