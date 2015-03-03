using System;
using System.Threading;

namespace GUI
{
    public static class Helper
    {
        public static void SynchronousInvoke(EventHandler e)
        {
            ManualResetEvent mr = new ManualResetEvent (false);
            Gtk.Application.Invoke (delegate(object sender, EventArgs args) {
                e (sender, args);
                mr.Set ();
            });
            mr.WaitOne ();
        }
    }
}

