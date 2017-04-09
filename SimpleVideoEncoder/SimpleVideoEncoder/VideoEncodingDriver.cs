/*
 @ 0xCCCCCCCC
*/

using System;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using System.Text.RegularExpressions;
using System.Threading;
using System.Threading.Tasks;
using Newtonsoft.Json.Linq;

namespace SimpleVideoEncoder
{
    public static class VideoEncodingDriver
    {
        private static readonly string FFmpegComponentsDir;
        public static event EventHandler EncodingSuccess;
        public static event EventHandler EncodingFailure;
        public static event EventHandler<Tuple<int, int>> EncodingProgressUpdated;

        static VideoEncodingDriver()
        {
            var appDir = Path.GetDirectoryName(Assembly.GetEntryAssembly().Location);
            Debug.Assert(appDir != null);
            FFmpegComponentsDir = Path.Combine(appDir, "ffmpeg");
        }

        // On UI-thread.
        public static void EncodeVideo(string videoPath)
        {
            Task.Run(()=>
            {
                Debug.WriteLine($"Running on thread {Thread.CurrentThread.ManagedThreadId}");
                var metaInfo = CheckVideoForEncoding(videoPath);
                var outputPath = GenerateOutputPath(videoPath);
                DoEncoding(videoPath, metaInfo, outputPath);
            });
        }

        private static VideoMetaInfo CheckVideoForEncoding(string videoPath)
        {
            var process = new Process {
                StartInfo = new ProcessStartInfo {
                    FileName = ProberPath,
                    Arguments = $"-show_streams -show_format -of json \"{videoPath}\"",
                    UseShellExecute = false,
                    RedirectStandardOutput = true,
                    CreateNoWindow = true
                }
            };

            process.Start();

            var probeResult = process.StandardOutput.ReadToEnd();

            process.WaitForExit();

            var resultData = JObject.Parse(probeResult);
            var streamData = resultData["streams"].Value<JArray>();
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

        private static void DoEncoding(string videoPath, VideoMetaInfo metaInfo, string outputPath)
        {
            var process = new Process {
                StartInfo = new ProcessStartInfo {
                    FileName = EncoderPath,
                    Arguments = GenerateEncodingCommand(videoPath, outputPath, 1),
                    UseShellExecute = false,
                    CreateNoWindow = true,
                    RedirectStandardError = true
                }
            };

            process.ErrorDataReceived += (sender, data)=>
            {
                if (data.Data != null)
                {
                    AnalyzeEncodingInfo(data, metaInfo, 1);
                }
            };

            process.Start();
            process.BeginErrorReadLine();

            process.WaitForExit();

            if (process.ExitCode != 0)
            {
                OnEncodingFailure();
                return;
            }

            process = new Process() {
                StartInfo = new ProcessStartInfo() {
                    FileName = EncoderPath,
                    Arguments = GenerateEncodingCommand(videoPath, outputPath, 2),
                    UseShellExecute = false,
                    CreateNoWindow = true,
                    RedirectStandardError = true
                }
            };

            process.ErrorDataReceived += (sender, data) =>
            {
                if (data.Data != null)
                {
                    AnalyzeEncodingInfo(data, metaInfo, 2);
                }
            };

            process.Start();
            process.BeginErrorReadLine();

            process.WaitForExit();

            OnEncodingSuccess();
        }

        private static void AnalyzeEncodingInfo(DataReceivedEventArgs data, VideoMetaInfo info, int currentPass)
        {
            Debug.WriteLine($"Analyzing on thread {Thread.CurrentThread.ManagedThreadId}");
            const string pat = @"frame= *(\d+) *fps= *(\d+\.?\d+).+time= *(\d+):(\d+):(\d+).+speed= *(\d+\.?\d+).*";
            var matches = Regex.Match(data.Data, pat);
            if (matches.Success)
            {
                // General formulas for calculating progress and remaining time:
                // Progress: ((cur_pass - 1) / total_pass) * 100 + num / dem * 100 / total_pass
                // ETA: ((total_pass / cur_pass) * total - elapsed) / velocity
                const int totalPass = 2;
                var percent = -1;
                var timeLeft = -1;
                if (info.FrameNum != 0)
                {
                    var totalFrameNum = info.FrameNum;
                    var frameNum = int.Parse(matches.Groups[1].Value);
                    var fps = double.Parse(matches.Groups[2].Value);
                    percent = Percentize(currentPass - 1, totalPass) +
                              Percentize(frameNum, totalFrameNum) / totalPass;
                    timeLeft = Math.Abs(fps) < 0.001 ? -1 :
                                    (int)((totalPass / currentPass * totalFrameNum - frameNum) / fps);
                }
                else if (info.Duration != 0)
                {
                    var duration = info.Duration;
                    var h = int.Parse(matches.Groups[3].Value);
                    var m = int.Parse(matches.Groups[4].Value);
                    var s = int.Parse(matches.Groups[5].Value);
                    var elapsed = h * 3600 + m * 60 + s;
                    var speed = double.Parse(matches.Groups[6].Value);
                    percent = Percentize(currentPass - 1, totalPass) + Percentize(elapsed, duration) / totalPass;
                    timeLeft = Math.Abs(speed) < 0.001 ? -1 :
                                    (int)((totalPass / currentPass * duration - elapsed) / speed);
                }

                OnEncodingProgressUpdated(Tuple.Create(percent, timeLeft));
            }
        }

        // Only do double-pass encoding.
        private static string GenerateEncodingCommand(string videoPath, string outputPath, int pass)
        {
            const string prefix = "ffmpeg_passlog_";
            const string cmdStr =
                "-i \"{0}\" -level:v 4.1 -x264opts merange=24:subme=10:aq-mode=3:aq-strength=0.8:min-keyint=1:" +
                "keyint=250:bframes=6:b-adapt=2:qcomp=0.7 -pix_fmt yuv420p -b:v 1785k -b:a 185k -ac 2 -ar 44100 " +
                "-threads 0 -vsync vfr -acodec aac -vcodec libx264 -passlogfile \"{1}\" -pass {2} -y \"{3}\"";
            var dir = Path.GetTempPath();
            var passLog = prefix + Path.GetFileNameWithoutExtension(videoPath);
            var passLogPath = Path.Combine(dir, passLog);

            return string.Format(cmdStr, videoPath, passLogPath, pass, outputPath);
        }

        private static string GenerateOutputPath(string srcPath)
        {
            var destDir = Path.GetDirectoryName(srcPath);
            Debug.Assert(destDir != null);

            var destFileName = Path.GetFileNameWithoutExtension(srcPath) + "_re.mp4";

            return Path.Combine(destDir, destFileName);
        }

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

        private static void OnEncodingSuccess()
        {
            EncodingSuccess?.Invoke(null, EventArgs.Empty);
        }

        private static void OnEncodingFailure()
        {
            EncodingFailure?.Invoke(null, EventArgs.Empty);
        }

        private static void OnEncodingProgressUpdated(Tuple<int, int> e)
        {
            EncodingProgressUpdated?.Invoke(null, e);
        }

        private static int Percentize(int numerator, int denominator)
        {
            return (int)((double)numerator / denominator * 100);
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
