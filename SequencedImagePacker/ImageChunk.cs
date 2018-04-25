using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace SequencedImagePacker
{
    struct ImageInfo
    {
        public ImageInfo(UInt32 offset, UInt32 size)
        {
            StartOffsetInChunk = offset;
            Size = size;
        }

        public UInt32 StartOffsetInChunk
        {
            get;set;
        }

        public UInt32 Size
        {
            get;set;
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
