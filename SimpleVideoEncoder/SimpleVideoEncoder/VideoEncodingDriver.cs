/*
 @ 0xCCCCCCCC
*/

using System;
using System.CodeDom;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using System.Security.Policy;
using Newtonsoft.Json.Linq;

namespace SimpleVideoEncoder
{
    public static class VideoEncodingDriver
    {
        private static readonly string FFmpegComponentsDir;

        static VideoEncodingDriver()
        {
            var appDir = Path.GetDirectoryName(Assembly.GetEntryAssembly().Location);
            Debug.Assert(appDir != null, "Failed to acquire executable path");
            FFmpegComponentsDir = Path.Combine(appDir, "ffmpeg");
        }

        private static VideoMetaInfo CheckVideoForEncoding(string videoPath)
        {
            var process = new Process {
                StartInfo = new ProcessStartInfo {
                    FileName = ProberPath,
                    Arguments = $"-show_streams -show_format -of json {videoPath}",
                    UseShellExecute = false,
                    RedirectStandardOutput = true,
                    CreateNoWindow = true
                }
            };

            process.Start();

            var probeResult = process.StandardOutput.ReadToEnd();

            process.WaitForExit();

            var resultData = JObject.Parse(probeResult);
            var streamData = resultData["stream"].Value<JArray>();
            if (streamData.Count < 2)
            {
                throw new InvalidVideoMetaInfo("Video is lack of video track or audio track!");
            }

            var videoMetaInfo = new VideoMetaInfo();
            foreach (var trackData in streamData)
            {
                if (trackData["codec_type"].Value<string>() == "video")
                {
                    videoMetaInfo.FrameNum = int.Parse(trackData["nb_frames"].Value<string>());
                    videoMetaInfo.Duration = (int)double.Parse(trackData["duration"].Value<string>());
                    break;
                }
            }

            if (!videoMetaInfo)
            {
                throw new InvalidVideoMetaInfo("Video meta info is incomplete!");
            }

            return videoMetaInfo;
        }

        public static void EncodeVideo(string videoPath)
        {}

        private static string ProberPath => Path.Combine(FFmpegComponentsDir, "ffprobe.exe");

        private static string EncoderPath => Path.Combine(FFmpegComponentsDir, "ffmpeg.exe");

        private struct VideoMetaInfo
        {
            public int FrameNum
            {
                get; set;
            }

            public int Duration
            {
                get; set;
            }

            public static implicit operator bool(VideoMetaInfo info)
            {
                return info.FrameNum != 0 && info.Duration != 0;
            }
        }
    }

    [Serializable]
    public class InvalidVideoMetaInfo : Exception
    {
        public InvalidVideoMetaInfo(string message)
            : base(message)
        {}

        public InvalidVideoMetaInfo(string message, Exception innerException)
            : base(message, innerException)
        {}
    }
}
