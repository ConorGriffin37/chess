using System;
using System.Diagnostics;

namespace GUI
{
    public class Engine
    {
        Process childProcess;
        string filename;

        public Engine (string filename)
        {
            childProcess = new Process ();
            this.filename = filename;
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
                output = childProcess.StandardOutput.ReadLine();
            } catch (Exception ex) {
                Console.Error.WriteLine ("(EE) Error receiving data from engine: " + ex.Message);
                throw new InvalidOperationException ("Child process not running.");
            }
            return output;
        }
    }
}

