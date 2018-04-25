using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SequencedImagePacker
{
    class Program
    {
        static void DisplayCollectionOrder(List<string> colDirs)
        {
            Console.WriteLine("Source Collection => Chunk #");
            for (int i = 0; i < colDirs.Count; ++i)
            {
                Console.WriteLine("{0} => {1}", Path.GetFileName(colDirs[i]), i);
            }
        }

        static void Main(string[] args)
        {
            var imgLibrary = @"d:\misc\imgpack";

            var collectionDirs = Directory.EnumerateDirectories(imgLibrary).ToList();

            DisplayCollectionOrder(collectionDirs);

            var chunk = ImageChunk.FromCollection(collectionDirs[0]);
            File.WriteAllBytes(Path.Combine(imgLibrary, "img.pak"), chunk.Marshal());
        }
    }
}
