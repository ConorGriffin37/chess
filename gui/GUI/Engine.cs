using System;
using System.Diagnostics;
using System.IO;
using System.Threading;

namespace GUI
{
    /**
     * @class Engine
     * @brief Class to represent an engine process.
     * 
     * @c Engine is a small wrapper for @c System.Process which controls
     * the engine process as a child process of the GUI. It sets up the
     * IO redirects necessary for the UCI protocol to function.
     */
    public class Engine
    {
        Process childProcess;
        string filename;

        public Engine (string filename)
        {
            childProcess = new Process ();
            if (File.Exists (filename)) {
                this.filename = filename;
            } else {
                throw new FileNotFoundException ("Engine file does not exist.", filename);
            }
        }

        public void Start()
        {
            try {
                childProcess.StartInfo.UseShellExecute = false;
                childProcess.StartInfo.FileName = filename;
                childProcess.StartInfo.CreateNoWindow = true;
                childProcess.StartInfo.RedirectStandardInput = true;
                childProcess.StartInfo.RedirectStandardOutput = true;
                childProcess.Start();
            } catch (Exception ex) {
                Console.Error.WriteLine ("(EE) Error starting engine process: " + ex.Message);
                throw new InvalidOperationException ("Process \"" + filename + "\" failed to start.");
            }
        }

        public void Write(string message)
        {
            try {
                childProcess.StandardInput.WriteLine (message);
            } catch (Exception ex) {
                Console.Error.WriteLine ("(EE) Error sending data to engine: " + ex.Message);
                throw new InvalidOperationException ("Child process not running.");
            }
        }

        public string Read()
        {
            string output;
            try {
                var readTask = childProcess.StandardOutput.ReadLineAsync();
                readTask.Wait(500);
                if(readTask.IsCompleted) {
                    output = readTask.Result;
                } else {
                    return null;
                }
            } catch(InvalidOperationException ex) {
                Console.Error.WriteLine ("(EE) Error receiving data from engine: " + ex.Message);
                Thread.Sleep (1000);
                return null;
            } catch (Exception ex) {
                Console.Error.WriteLine ("(EE) Error receiving data from engine: " + ex.Message);
                throw new InvalidOperationException ("Child process not running.");
            }
            return output;
        }
    }
}

