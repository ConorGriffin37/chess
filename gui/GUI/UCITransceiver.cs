using System;
using System.IO;

namespace GUI
{
    /**
     * @class UCITransceiver
     * @brief Sends and receives UCI commands.
     * 
     * This class contains an @c Engine object which it uses to send
     * and receive UCI commands.
     * 
     * @see Engine
     */
    public class UCITransceiver
    {
        Engine engine;
        public string EngineName { get; private set; }
        public string EngineAuthor { get; private set; }
        public bool IsThinking { get; private set; }

        private int number;

        public UCITransceiver (string engineFilename, int number)
        {
            try {
                engine = new Engine(engineFilename);
                this.number = number;
            } catch(FileNotFoundException) {
                throw new ArgumentException ("Bad engine filename provided.", "engineFilename");
            }
            engine.Start ();
        }

        public void Init()
        {
            engine.Write("uci");
            string response;
            do {
                response = engine.Read();
                Debug.Log(response);
                if(response.StartsWith("id name ")) {
                    EngineName = response.Substring(8);
                } else if(response.StartsWith("id author ")) {
                    EngineAuthor = response.Substring(10);
                } else if(response == "uciok") {
                    // We don't require Helper.SynchronousInvoke() in Init() because
                    // it will always run in the main thread.
                    MainClass.win.LogEngineNameAndAuthor(number, EngineName, EngineAuthor);
                    return;
                }
            } while(response != null);
        }

        /**
         * @fn SendPosition
         * @brief Sends the current board position to the engine.
         * 
         * @param position either a valid FEN string or "startpos" to use the default starting position.
         * @param moves a string of space-delimited moves made since @c position. Optional.
         */
        public void SendPosition(string position, string moves = null)
        {
            string output;
            if (position == "startpos") {
                output = "position startpos";
            } else {
                output = "position fen " + position;
            }
            if (moves != null) {
                output += " moves " + moves;
            }
            Debug.Log ("Position: " + position);
            engine.Write (output);
        }

        public string Go(string time = "infinite") {
            Helper.SynchronousInvoke (delegate {
                MainClass.win.ClearEngineOutput();
            });
            Debug.Log ("go " + time);
            engine.Write("go " + time);
            IsThinking = true;

            string response;
            do {
                if(MainClass.EngineStopTokenSource.IsCancellationRequested) {
                    Debug.Log ("Engine task cancelled.");
                    MainClass.ResetEngineStopTokenSource();
                    return null;
                }
                response = engine.Read ();
                if(response == null) continue;
                Debug.Log(response);
                Helper.SynchronousInvoke(delegate {
                    MainClass.win.LogEngineOutput(response);
                });
                if (response.StartsWith ("bestmove")) {
                    IsThinking = false;
                    if(response.Substring(9).Length > 4) {
                        return response.Substring (9, 5);
                    } else {
                        return response.Substring (9, 4);
                    }
                }
            } while(true);
        }

        public string StopAndGetBestMove()
        {
            engine.Write("stop");
            IsThinking = false;
            string response;
            do {
                response = engine.Read ();
                Debug.Log(response);
                if (response.StartsWith ("bestmove")) {
                    WaitUntilReady();
                    Debug.Log ("Engine stopped and ready for new input.");
                    return response.Substring (9, 5);
                }
            } while(response != null);
            throw new TimeoutException ("Engine has stopped responding.");
        }

        public void WaitUntilReady()
        {
            engine.Write("isready");
            string response;
            do {
                response = engine.Read ();
                Debug.Log(response);
                if (response == "readyok") {
                    return;
                }
            } while(response != null);
        }

        public void Quit()
        {
            engine.Write("quit");
        }

        public void StopAndIgnoreMove()
        {
            engine.Write("stop");
            IsThinking = false;
            /*
            // Wait until the EngineStopTokenSource has been reset.
            // This indicates that the task has in fact stopped and we can now proceed as normal.
            while (MainClass.EngineStopTokenSource.IsCancellationRequested)
                continue;
            */
            WaitUntilReady ();
            Debug.Log ("Engine stopped and ready for new input.");
            MainClass.ResetEngineStopTokenSource ();
        }
    }
}

