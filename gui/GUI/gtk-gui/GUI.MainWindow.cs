
// This file has been generated by the GUI designer. Do not modify.
namespace GUI
{
	public partial class MainWindow
	{
		private global::Gtk.UIManager UIManager;
		
		private global::Gtk.Action FileAction;
		
		private global::Gtk.Action BoardAction;
		
		private global::Gtk.Action EngineAction;
		
		private global::Gtk.Action HelpAction;
		
		private global::Gtk.Action QuitAction;
		
		private global::Gtk.Action LoadFENAction;
		
		private global::Gtk.Action LoadEngineAction;
		
		private global::Gtk.Action AboutAction;
		
		private global::Gtk.Action ResetBoardAction;
		
		private global::Gtk.Action ResetBoardAction1;
		
		private global::Gtk.Action MakeEngineMoveAction;
		
		private global::Gtk.Action FlipBoardAction;
		
		private global::Gtk.Action GameAction;
		
		private global::Gtk.Action SetClockAction;
		
		private global::Gtk.VBox vbox1;
		
		private global::Gtk.MenuBar MenuBar;
		
		private global::Gtk.HBox hbox3;
		
		private global::Gtk.VBox vbox2;
		
		private global::Gtk.DrawingArea BoardArea;
		
		private global::Gtk.VBox vbox3;
		
		private global::Gtk.HBox hbox4;
		
		private global::Gtk.VBox vbox4;
		
		private global::Gtk.Label WhiteClockName;
		
		private global::Gtk.Label WhiteClockLabel;
		
		private global::Gtk.VBox vbox5;
		
		private global::Gtk.Label BlackClockTime;
		
		private global::Gtk.Label BlackClockLabel;
		
		private global::Gtk.HBox hbox1;
		
		private global::Gtk.Label EngineNameLabel;
		
		private global::Gtk.Label EngineAuthorLabel;
		
		private global::Gtk.Label EngineDepthLabel;
		
		private global::Gtk.Label EngineNPSLabel;
		
		private global::Gtk.ScrolledWindow GtkScrolledWindow;
		
		private global::Gtk.TextView EngineOutput;

		protected virtual void Build ()
		{
			global::Stetic.Gui.Initialize (this);
			// Widget GUI.MainWindow
			this.UIManager = new global::Gtk.UIManager ();
			global::Gtk.ActionGroup w1 = new global::Gtk.ActionGroup ("Default");
			this.FileAction = new global::Gtk.Action ("FileAction", global::Mono.Unix.Catalog.GetString ("_File"), null, null);
			this.FileAction.ShortLabel = global::Mono.Unix.Catalog.GetString ("_File");
			w1.Add (this.FileAction, "<Alt><Mod2>f");
			this.BoardAction = new global::Gtk.Action ("BoardAction", global::Mono.Unix.Catalog.GetString ("_Board"), null, null);
			this.BoardAction.ShortLabel = global::Mono.Unix.Catalog.GetString ("_Board");
			w1.Add (this.BoardAction, "<Alt><Mod2>b");
			this.EngineAction = new global::Gtk.Action ("EngineAction", global::Mono.Unix.Catalog.GetString ("_Engine"), null, null);
			this.EngineAction.ShortLabel = global::Mono.Unix.Catalog.GetString ("_Engine");
			w1.Add (this.EngineAction, "<Alt><Mod2>e");
			this.HelpAction = new global::Gtk.Action ("HelpAction", global::Mono.Unix.Catalog.GetString ("_Help"), null, null);
			this.HelpAction.ShortLabel = global::Mono.Unix.Catalog.GetString ("_Help");
			w1.Add (this.HelpAction, "<Alt><Mod2>h");
			this.QuitAction = new global::Gtk.Action ("QuitAction", global::Mono.Unix.Catalog.GetString ("_Quit"), null, null);
			this.QuitAction.ShortLabel = global::Mono.Unix.Catalog.GetString ("_Quit");
			w1.Add (this.QuitAction, "<Primary><Mod2>q");
			this.LoadFENAction = new global::Gtk.Action ("LoadFENAction", global::Mono.Unix.Catalog.GetString ("Load _FEN"), null, null);
			this.LoadFENAction.ShortLabel = global::Mono.Unix.Catalog.GetString ("Load _FEN");
			w1.Add (this.LoadFENAction, "<Primary><Mod2>f");
			this.LoadEngineAction = new global::Gtk.Action ("LoadEngineAction", global::Mono.Unix.Catalog.GetString ("_Load Engine"), null, null);
			this.LoadEngineAction.ShortLabel = global::Mono.Unix.Catalog.GetString ("_Load Engine");
			w1.Add (this.LoadEngineAction, "<Primary><Mod2>e");
			this.AboutAction = new global::Gtk.Action ("AboutAction", global::Mono.Unix.Catalog.GetString ("_About"), null, null);
			this.AboutAction.ShortLabel = global::Mono.Unix.Catalog.GetString ("_About");
			w1.Add (this.AboutAction, "<Primary><Mod2>a");
			this.ResetBoardAction = new global::Gtk.Action ("ResetBoardAction", global::Mono.Unix.Catalog.GetString ("Reset Board"), null, null);
			this.ResetBoardAction.ShortLabel = global::Mono.Unix.Catalog.GetString ("Reset Board");
			w1.Add (this.ResetBoardAction, null);
			this.ResetBoardAction1 = new global::Gtk.Action ("ResetBoardAction1", global::Mono.Unix.Catalog.GetString ("_Reset Board"), null, null);
			this.ResetBoardAction1.ShortLabel = global::Mono.Unix.Catalog.GetString ("_Reset Board");
			w1.Add (this.ResetBoardAction1, "<Primary>r");
			this.MakeEngineMoveAction = new global::Gtk.Action ("MakeEngineMoveAction", global::Mono.Unix.Catalog.GetString ("Make Engine _Move"), null, null);
			this.MakeEngineMoveAction.ShortLabel = global::Mono.Unix.Catalog.GetString ("Make Engine _Move");
			w1.Add (this.MakeEngineMoveAction, "<Primary>m");
			this.FlipBoardAction = new global::Gtk.Action ("FlipBoardAction", global::Mono.Unix.Catalog.GetString ("Flip Board"), null, null);
			this.FlipBoardAction.ShortLabel = global::Mono.Unix.Catalog.GetString ("Flip Board");
			w1.Add (this.FlipBoardAction, null);
			this.GameAction = new global::Gtk.Action ("GameAction", global::Mono.Unix.Catalog.GetString ("Game"), null, null);
			this.GameAction.ShortLabel = global::Mono.Unix.Catalog.GetString ("Game");
			w1.Add (this.GameAction, null);
			this.SetClockAction = new global::Gtk.Action ("SetClockAction", global::Mono.Unix.Catalog.GetString ("Set _Clock"), null, null);
			this.SetClockAction.ShortLabel = global::Mono.Unix.Catalog.GetString ("Set _Clock");
			w1.Add (this.SetClockAction, "<Primary>c");
			this.UIManager.InsertActionGroup (w1, 0);
			this.AddAccelGroup (this.UIManager.AccelGroup);
			this.Name = "GUI.MainWindow";
			this.Title = global::Mono.Unix.Catalog.GetString ("Gandalf Chess GUI");
			this.WindowPosition = ((global::Gtk.WindowPosition)(4));
			// Container child GUI.MainWindow.Gtk.Container+ContainerChild
			this.vbox1 = new global::Gtk.VBox ();
			this.vbox1.Name = "vbox1";
			this.vbox1.Spacing = 6;
			// Container child vbox1.Gtk.Box+BoxChild
			this.UIManager.AddUiFromString ("<ui><menubar name='MenuBar'><menu name='FileAction' action='FileAction'><menuitem name='QuitAction' action='QuitAction'/></menu><menu name='BoardAction' action='BoardAction'><menuitem name='ResetBoardAction1' action='ResetBoardAction1'/><menuitem name='FlipBoardAction' action='FlipBoardAction'/><menuitem name='LoadFENAction' action='LoadFENAction'/></menu><menu name='GameAction' action='GameAction'><menuitem name='SetClockAction' action='SetClockAction'/></menu><menu name='EngineAction' action='EngineAction'><menuitem name='LoadEngineAction' action='LoadEngineAction'/><menuitem name='MakeEngineMoveAction' action='MakeEngineMoveAction'/></menu><menu name='HelpAction' action='HelpAction'><menuitem name='AboutAction' action='AboutAction'/></menu></menubar></ui>");
			this.MenuBar = ((global::Gtk.MenuBar)(this.UIManager.GetWidget ("/MenuBar")));
			this.MenuBar.Name = "MenuBar";
			this.vbox1.Add (this.MenuBar);
			global::Gtk.Box.BoxChild w2 = ((global::Gtk.Box.BoxChild)(this.vbox1 [this.MenuBar]));
			w2.Position = 0;
			w2.Expand = false;
			w2.Fill = false;
			// Container child vbox1.Gtk.Box+BoxChild
			this.hbox3 = new global::Gtk.HBox ();
			this.hbox3.Name = "hbox3";
			this.hbox3.Spacing = 3;
			// Container child hbox3.Gtk.Box+BoxChild
			this.vbox2 = new global::Gtk.VBox ();
			this.vbox2.Name = "vbox2";
			this.vbox2.Spacing = 6;
			// Container child vbox2.Gtk.Box+BoxChild
			this.BoardArea = new global::Gtk.DrawingArea ();
			this.BoardArea.WidthRequest = 570;
			this.BoardArea.HeightRequest = 550;
			this.BoardArea.Name = "BoardArea";
			this.vbox2.Add (this.BoardArea);
			global::Gtk.Box.BoxChild w3 = ((global::Gtk.Box.BoxChild)(this.vbox2 [this.BoardArea]));
			w3.Position = 0;
			this.hbox3.Add (this.vbox2);
			global::Gtk.Box.BoxChild w4 = ((global::Gtk.Box.BoxChild)(this.hbox3 [this.vbox2]));
			w4.Position = 0;
			// Container child hbox3.Gtk.Box+BoxChild
			this.vbox3 = new global::Gtk.VBox ();
			this.vbox3.WidthRequest = 200;
			this.vbox3.Name = "vbox3";
			this.vbox3.Spacing = 6;
			// Container child vbox3.Gtk.Box+BoxChild
			this.hbox4 = new global::Gtk.HBox ();
			this.hbox4.Name = "hbox4";
			this.hbox4.Spacing = 6;
			// Container child hbox4.Gtk.Box+BoxChild
			this.vbox4 = new global::Gtk.VBox ();
			this.vbox4.Name = "vbox4";
			// Container child vbox4.Gtk.Box+BoxChild
			this.WhiteClockName = new global::Gtk.Label ();
			this.WhiteClockName.Name = "WhiteClockName";
			this.WhiteClockName.Xpad = 30;
			this.WhiteClockName.Ypad = 5;
			this.WhiteClockName.LabelProp = global::Mono.Unix.Catalog.GetString ("White");
			this.WhiteClockName.Justify = ((global::Gtk.Justification)(2));
			this.vbox4.Add (this.WhiteClockName);
			global::Gtk.Box.BoxChild w5 = ((global::Gtk.Box.BoxChild)(this.vbox4 [this.WhiteClockName]));
			w5.Position = 0;
			w5.Expand = false;
			w5.Fill = false;
			// Container child vbox4.Gtk.Box+BoxChild
			this.WhiteClockLabel = new global::Gtk.Label ();
			this.WhiteClockLabel.Name = "WhiteClockLabel";
			this.WhiteClockLabel.Ypad = 5;
			this.WhiteClockLabel.LabelProp = global::Mono.Unix.Catalog.GetString ("white_clock_label");
			this.vbox4.Add (this.WhiteClockLabel);
			global::Gtk.Box.BoxChild w6 = ((global::Gtk.Box.BoxChild)(this.vbox4 [this.WhiteClockLabel]));
			w6.Position = 1;
			w6.Expand = false;
			w6.Fill = false;
			this.hbox4.Add (this.vbox4);
			global::Gtk.Box.BoxChild w7 = ((global::Gtk.Box.BoxChild)(this.hbox4 [this.vbox4]));
			w7.Position = 0;
			w7.Expand = false;
			w7.Fill = false;
			// Container child hbox4.Gtk.Box+BoxChild
			this.vbox5 = new global::Gtk.VBox ();
			this.vbox5.Name = "vbox5";
			// Container child vbox5.Gtk.Box+BoxChild
			this.BlackClockTime = new global::Gtk.Label ();
			this.BlackClockTime.Name = "BlackClockTime";
			this.BlackClockTime.Xpad = 30;
			this.BlackClockTime.Ypad = 5;
			this.BlackClockTime.LabelProp = global::Mono.Unix.Catalog.GetString ("Black");
			this.vbox5.Add (this.BlackClockTime);
			global::Gtk.Box.BoxChild w8 = ((global::Gtk.Box.BoxChild)(this.vbox5 [this.BlackClockTime]));
			w8.Position = 0;
			w8.Expand = false;
			w8.Fill = false;
			// Container child vbox5.Gtk.Box+BoxChild
			this.BlackClockLabel = new global::Gtk.Label ();
			this.BlackClockLabel.Name = "BlackClockLabel";
			this.BlackClockLabel.Ypad = 5;
			this.BlackClockLabel.LabelProp = global::Mono.Unix.Catalog.GetString ("black_clock_label");
			this.vbox5.Add (this.BlackClockLabel);
			global::Gtk.Box.BoxChild w9 = ((global::Gtk.Box.BoxChild)(this.vbox5 [this.BlackClockLabel]));
			w9.Position = 1;
			w9.Expand = false;
			w9.Fill = false;
			this.hbox4.Add (this.vbox5);
			global::Gtk.Box.BoxChild w10 = ((global::Gtk.Box.BoxChild)(this.hbox4 [this.vbox5]));
			w10.Position = 1;
			w10.Expand = false;
			w10.Fill = false;
			this.vbox3.Add (this.hbox4);
			global::Gtk.Box.BoxChild w11 = ((global::Gtk.Box.BoxChild)(this.vbox3 [this.hbox4]));
			w11.Position = 0;
			w11.Expand = false;
			w11.Fill = false;
			this.hbox3.Add (this.vbox3);
			global::Gtk.Box.BoxChild w12 = ((global::Gtk.Box.BoxChild)(this.hbox3 [this.vbox3]));
			w12.Position = 1;
			this.vbox1.Add (this.hbox3);
			global::Gtk.Box.BoxChild w13 = ((global::Gtk.Box.BoxChild)(this.vbox1 [this.hbox3]));
			w13.Position = 1;
			// Container child vbox1.Gtk.Box+BoxChild
			this.hbox1 = new global::Gtk.HBox ();
			this.hbox1.Name = "hbox1";
			// Container child hbox1.Gtk.Box+BoxChild
			this.EngineNameLabel = new global::Gtk.Label ();
			this.EngineNameLabel.Name = "EngineNameLabel";
			this.EngineNameLabel.Xpad = 10;
			this.EngineNameLabel.LabelProp = global::Mono.Unix.Catalog.GetString ("engine_name");
			this.hbox1.Add (this.EngineNameLabel);
			global::Gtk.Box.BoxChild w14 = ((global::Gtk.Box.BoxChild)(this.hbox1 [this.EngineNameLabel]));
			w14.Position = 0;
			w14.Expand = false;
			w14.Fill = false;
			// Container child hbox1.Gtk.Box+BoxChild
			this.EngineAuthorLabel = new global::Gtk.Label ();
			this.EngineAuthorLabel.Name = "EngineAuthorLabel";
			this.EngineAuthorLabel.Xpad = 10;
			this.EngineAuthorLabel.LabelProp = global::Mono.Unix.Catalog.GetString ("engine_author");
			this.hbox1.Add (this.EngineAuthorLabel);
			global::Gtk.Box.BoxChild w15 = ((global::Gtk.Box.BoxChild)(this.hbox1 [this.EngineAuthorLabel]));
			w15.Position = 1;
			w15.Expand = false;
			w15.Fill = false;
			// Container child hbox1.Gtk.Box+BoxChild
			this.EngineDepthLabel = new global::Gtk.Label ();
			this.EngineDepthLabel.Name = "EngineDepthLabel";
			this.EngineDepthLabel.Xpad = 10;
			this.EngineDepthLabel.LabelProp = global::Mono.Unix.Catalog.GetString ("engine_depth");
			this.hbox1.Add (this.EngineDepthLabel);
			global::Gtk.Box.BoxChild w16 = ((global::Gtk.Box.BoxChild)(this.hbox1 [this.EngineDepthLabel]));
			w16.Position = 2;
			w16.Expand = false;
			w16.Fill = false;
			// Container child hbox1.Gtk.Box+BoxChild
			this.EngineNPSLabel = new global::Gtk.Label ();
			this.EngineNPSLabel.Name = "EngineNPSLabel";
			this.EngineNPSLabel.Xpad = 10;
			this.EngineNPSLabel.LabelProp = global::Mono.Unix.Catalog.GetString ("engine_nps");
			this.hbox1.Add (this.EngineNPSLabel);
			global::Gtk.Box.BoxChild w17 = ((global::Gtk.Box.BoxChild)(this.hbox1 [this.EngineNPSLabel]));
			w17.Position = 3;
			w17.Expand = false;
			w17.Fill = false;
			this.vbox1.Add (this.hbox1);
			global::Gtk.Box.BoxChild w18 = ((global::Gtk.Box.BoxChild)(this.vbox1 [this.hbox1]));
			w18.Position = 2;
			w18.Expand = false;
			w18.Fill = false;
			// Container child vbox1.Gtk.Box+BoxChild
			this.GtkScrolledWindow = new global::Gtk.ScrolledWindow ();
			this.GtkScrolledWindow.Name = "GtkScrolledWindow";
			this.GtkScrolledWindow.ShadowType = ((global::Gtk.ShadowType)(1));
			// Container child GtkScrolledWindow.Gtk.Container+ContainerChild
			this.EngineOutput = new global::Gtk.TextView ();
			this.EngineOutput.HeightRequest = 30;
			this.EngineOutput.CanFocus = true;
			this.EngineOutput.Name = "EngineOutput";
			this.EngineOutput.Editable = false;
			this.EngineOutput.CursorVisible = false;
			this.EngineOutput.WrapMode = ((global::Gtk.WrapMode)(2));
			this.EngineOutput.LeftMargin = 5;
			this.GtkScrolledWindow.Add (this.EngineOutput);
			this.vbox1.Add (this.GtkScrolledWindow);
			global::Gtk.Box.BoxChild w20 = ((global::Gtk.Box.BoxChild)(this.vbox1 [this.GtkScrolledWindow]));
			w20.Position = 3;
			this.Add (this.vbox1);
			if ((this.Child != null)) {
				this.Child.ShowAll ();
			}
			this.DefaultWidth = 773;
			this.DefaultHeight = 643;
			this.Show ();
			this.DeleteEvent += new global::Gtk.DeleteEventHandler (this.OnDeleteEvent);
			this.QuitAction.Activated += new global::System.EventHandler (this.OnQuit);
			this.LoadFENAction.Activated += new global::System.EventHandler (this.OnLoadFEN);
			this.LoadEngineAction.Activated += new global::System.EventHandler (this.OnLoadEngine);
			this.AboutAction.Activated += new global::System.EventHandler (this.OnAbout);
			this.ResetBoardAction1.Activated += new global::System.EventHandler (this.OnResetBoard);
			this.MakeEngineMoveAction.Activated += new global::System.EventHandler (this.OnMakeEngineMove);
			this.FlipBoardAction.Activated += new global::System.EventHandler (this.OnFlipBoard);
			this.SetClockAction.Activated += new global::System.EventHandler (this.OnSetClock);
			this.BoardArea.ExposeEvent += new global::Gtk.ExposeEventHandler (this.OnBoardExpose);
			this.BoardArea.ButtonPressEvent += new global::Gtk.ButtonPressEventHandler (this.OnPieceClick);
		}
	}
}
