
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
		
		private global::Gtk.Action LoadEngineOneAction;
		
		private global::Gtk.Action AboutAction;
		
		private global::Gtk.Action ResetBoardAction;
		
		private global::Gtk.Action ResetBoardAction1;
		
		private global::Gtk.Action MakeEngineMoveAction;
		
		private global::Gtk.Action FlipBoardAction;
		
		private global::Gtk.Action GameAction;
		
		private global::Gtk.Action SetClockAction;
		
		private global::Gtk.Action SetEngineStrengthAction;
		
		private global::Gtk.Action GameModeAction;
		
		private global::Gtk.RadioAction OnePlayerAction;
		
		private global::Gtk.RadioAction TwoPlayerAction;
		
		private global::Gtk.RadioAction EnginesAction;
		
		private global::Gtk.Action AnalysePositionAction;
		
		private global::Gtk.Action LoadEngine2Action;
		
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
		
		private global::Gtk.Label MaterialDifferenceLabel;
		
		private global::Gtk.Label PlayerToMoveLabel;
		
		private global::Gtk.HBox hbox1;
		
		private global::Gtk.Label EngineOneNameLabel;
		
		private global::Gtk.Label EngineOneAuthorLabel;
		
		private global::Gtk.Label EngineOneDepthLabel;
		
		private global::Gtk.Label EngineOneNPSLabel;
		
		private global::Gtk.HBox hbox2;
		
		private global::Gtk.Label EngineTwoNameLabel;
		
		private global::Gtk.Label EngineTwoAuthorLabel;
		
		private global::Gtk.Label EngineTwoDepthLabel;
		
		private global::Gtk.Label EngineTwoNPSLabel;
		
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
			this.LoadEngineOneAction = new global::Gtk.Action ("LoadEngineOneAction", global::Mono.Unix.Catalog.GetString ("Load Engine 1"), null, null);
			this.LoadEngineOneAction.ShortLabel = global::Mono.Unix.Catalog.GetString ("Load Engine");
			w1.Add (this.LoadEngineOneAction, "<Primary><Mod2>1");
			this.AboutAction = new global::Gtk.Action ("AboutAction", global::Mono.Unix.Catalog.GetString ("_About"), null, null);
			this.AboutAction.ShortLabel = global::Mono.Unix.Catalog.GetString ("_About");
			w1.Add (this.AboutAction, null);
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
			this.SetEngineStrengthAction = new global::Gtk.Action ("SetEngineStrengthAction", global::Mono.Unix.Catalog.GetString ("Set Engine Strength"), null, null);
			this.SetEngineStrengthAction.ShortLabel = global::Mono.Unix.Catalog.GetString ("Set Engine Strength");
			w1.Add (this.SetEngineStrengthAction, null);
			this.GameModeAction = new global::Gtk.Action ("GameModeAction", global::Mono.Unix.Catalog.GetString ("Game Mode"), null, null);
			this.GameModeAction.ShortLabel = global::Mono.Unix.Catalog.GetString ("Game Mode");
			w1.Add (this.GameModeAction, null);
			this.OnePlayerAction = new global::Gtk.RadioAction ("OnePlayerAction", global::Mono.Unix.Catalog.GetString ("Player vs Engine"), null, null, 0);
			this.OnePlayerAction.Group = new global::GLib.SList (global::System.IntPtr.Zero);
			this.OnePlayerAction.ShortLabel = global::Mono.Unix.Catalog.GetString ("Player vs Engine");
			w1.Add (this.OnePlayerAction, null);
			this.TwoPlayerAction = new global::Gtk.RadioAction ("TwoPlayerAction", global::Mono.Unix.Catalog.GetString ("Player vs Player"), null, null, 0);
			this.TwoPlayerAction.Group = this.OnePlayerAction.Group;
			this.TwoPlayerAction.ShortLabel = global::Mono.Unix.Catalog.GetString ("Player vs Player");
			w1.Add (this.TwoPlayerAction, null);
			this.EnginesAction = new global::Gtk.RadioAction ("EnginesAction", global::Mono.Unix.Catalog.GetString ("Engine vs Engine"), null, null, 0);
			this.EnginesAction.Group = this.OnePlayerAction.Group;
			this.EnginesAction.ShortLabel = global::Mono.Unix.Catalog.GetString ("Engine vs Engine");
			w1.Add (this.EnginesAction, null);
			this.AnalysePositionAction = new global::Gtk.Action ("AnalysePositionAction", global::Mono.Unix.Catalog.GetString ("_Analyse Position"), null, null);
			this.AnalysePositionAction.ShortLabel = global::Mono.Unix.Catalog.GetString ("_Analyse Position");
			w1.Add (this.AnalysePositionAction, "<Primary>a");
			this.LoadEngine2Action = new global::Gtk.Action ("LoadEngine2Action", global::Mono.Unix.Catalog.GetString ("Load Engine 2"), null, null);
			this.LoadEngine2Action.ShortLabel = global::Mono.Unix.Catalog.GetString ("Load Engine 2");
			w1.Add (this.LoadEngine2Action, "<Primary><Mod2>2");
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
			this.UIManager.AddUiFromString ("<ui><menubar name='MenuBar'><menu name='FileAction' action='FileAction'><menuitem name='QuitAction' action='QuitAction'/></menu><menu name='GameAction' action='GameAction'><menuitem name='SetClockAction' action='SetClockAction'/><menu name='GameModeAction' action='GameModeAction'><menuitem name='OnePlayerAction' action='OnePlayerAction'/><menuitem name='TwoPlayerAction' action='TwoPlayerAction'/><menuitem name='EnginesAction' action='EnginesAction'/></menu></menu><menu name='BoardAction' action='BoardAction'><menuitem name='ResetBoardAction1' action='ResetBoardAction1'/><menuitem name='FlipBoardAction' action='FlipBoardAction'/><menuitem name='LoadFENAction' action='LoadFENAction'/></menu><menu name='EngineAction' action='EngineAction'><menuitem name='LoadEngineOneAction' action='LoadEngineOneAction'/><menuitem name='LoadEngine2Action' action='LoadEngine2Action'/><menuitem name='MakeEngineMoveAction' action='MakeEngineMoveAction'/><menuitem name='SetEngineStrengthAction' action='SetEngineStrengthAction'/><menuitem name='AnalysePositionAction' action='AnalysePositionAction'/></menu><menu name='HelpAction' action='HelpAction'><menuitem name='AboutAction' action='AboutAction'/></menu></menubar></ui>");
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
			// Container child vbox3.Gtk.Box+BoxChild
			this.MaterialDifferenceLabel = new global::Gtk.Label ();
			this.MaterialDifferenceLabel.Name = "MaterialDifferenceLabel";
			this.MaterialDifferenceLabel.Ypad = 20;
			this.MaterialDifferenceLabel.LabelProp = global::Mono.Unix.Catalog.GetString ("<material_difference>");
			this.MaterialDifferenceLabel.Wrap = true;
			this.vbox3.Add (this.MaterialDifferenceLabel);
			global::Gtk.Box.BoxChild w12 = ((global::Gtk.Box.BoxChild)(this.vbox3 [this.MaterialDifferenceLabel]));
			w12.Position = 1;
			w12.Expand = false;
			w12.Fill = false;
			// Container child vbox3.Gtk.Box+BoxChild
			this.PlayerToMoveLabel = new global::Gtk.Label ();
			this.PlayerToMoveLabel.Name = "PlayerToMoveLabel";
			this.PlayerToMoveLabel.Ypad = 10;
			this.PlayerToMoveLabel.LabelProp = global::Mono.Unix.Catalog.GetString ("Player to move: White");
			this.vbox3.Add (this.PlayerToMoveLabel);
			global::Gtk.Box.BoxChild w13 = ((global::Gtk.Box.BoxChild)(this.vbox3 [this.PlayerToMoveLabel]));
			w13.Position = 2;
			w13.Expand = false;
			w13.Fill = false;
			this.hbox3.Add (this.vbox3);
			global::Gtk.Box.BoxChild w14 = ((global::Gtk.Box.BoxChild)(this.hbox3 [this.vbox3]));
			w14.Position = 1;
			this.vbox1.Add (this.hbox3);
			global::Gtk.Box.BoxChild w15 = ((global::Gtk.Box.BoxChild)(this.vbox1 [this.hbox3]));
			w15.Position = 1;
			// Container child vbox1.Gtk.Box+BoxChild
			this.hbox1 = new global::Gtk.HBox ();
			this.hbox1.Name = "hbox1";
			this.hbox1.Spacing = 5;
			// Container child hbox1.Gtk.Box+BoxChild
			this.EngineOneNameLabel = new global::Gtk.Label ();
			this.EngineOneNameLabel.Name = "EngineOneNameLabel";
			this.EngineOneNameLabel.Xpad = 10;
			this.EngineOneNameLabel.LabelProp = global::Mono.Unix.Catalog.GetString ("engine_name");
			this.hbox1.Add (this.EngineOneNameLabel);
			global::Gtk.Box.BoxChild w16 = ((global::Gtk.Box.BoxChild)(this.hbox1 [this.EngineOneNameLabel]));
			w16.Position = 0;
			w16.Expand = false;
			w16.Fill = false;
			// Container child hbox1.Gtk.Box+BoxChild
			this.EngineOneAuthorLabel = new global::Gtk.Label ();
			this.EngineOneAuthorLabel.Name = "EngineOneAuthorLabel";
			this.EngineOneAuthorLabel.Xpad = 10;
			this.EngineOneAuthorLabel.LabelProp = global::Mono.Unix.Catalog.GetString ("engine_author");
			this.hbox1.Add (this.EngineOneAuthorLabel);
			global::Gtk.Box.BoxChild w17 = ((global::Gtk.Box.BoxChild)(this.hbox1 [this.EngineOneAuthorLabel]));
			w17.Position = 1;
			// Container child hbox1.Gtk.Box+BoxChild
			this.EngineOneDepthLabel = new global::Gtk.Label ();
			this.EngineOneDepthLabel.Name = "EngineOneDepthLabel";
			this.EngineOneDepthLabel.Xpad = 10;
			this.EngineOneDepthLabel.LabelProp = global::Mono.Unix.Catalog.GetString ("engine_depth");
			this.hbox1.Add (this.EngineOneDepthLabel);
			global::Gtk.Box.BoxChild w18 = ((global::Gtk.Box.BoxChild)(this.hbox1 [this.EngineOneDepthLabel]));
			w18.Position = 2;
			// Container child hbox1.Gtk.Box+BoxChild
			this.EngineOneNPSLabel = new global::Gtk.Label ();
			this.EngineOneNPSLabel.Name = "EngineOneNPSLabel";
			this.EngineOneNPSLabel.Xpad = 10;
			this.EngineOneNPSLabel.LabelProp = global::Mono.Unix.Catalog.GetString ("engine_nps");
			this.hbox1.Add (this.EngineOneNPSLabel);
			global::Gtk.Box.BoxChild w19 = ((global::Gtk.Box.BoxChild)(this.hbox1 [this.EngineOneNPSLabel]));
			w19.Position = 3;
			this.vbox1.Add (this.hbox1);
			global::Gtk.Box.BoxChild w20 = ((global::Gtk.Box.BoxChild)(this.vbox1 [this.hbox1]));
			w20.Position = 2;
			w20.Expand = false;
			w20.Fill = false;
			// Container child vbox1.Gtk.Box+BoxChild
			this.hbox2 = new global::Gtk.HBox ();
			this.hbox2.Name = "hbox2";
			this.hbox2.Spacing = 5;
			// Container child hbox2.Gtk.Box+BoxChild
			this.EngineTwoNameLabel = new global::Gtk.Label ();
			this.EngineTwoNameLabel.Name = "EngineTwoNameLabel";
			this.EngineTwoNameLabel.Xpad = 10;
			this.EngineTwoNameLabel.LabelProp = global::Mono.Unix.Catalog.GetString ("engine_name");
			this.hbox2.Add (this.EngineTwoNameLabel);
			global::Gtk.Box.BoxChild w21 = ((global::Gtk.Box.BoxChild)(this.hbox2 [this.EngineTwoNameLabel]));
			w21.Position = 0;
			w21.Expand = false;
			w21.Fill = false;
			// Container child hbox2.Gtk.Box+BoxChild
			this.EngineTwoAuthorLabel = new global::Gtk.Label ();
			this.EngineTwoAuthorLabel.Name = "EngineTwoAuthorLabel";
			this.EngineTwoAuthorLabel.Xpad = 10;
			this.EngineTwoAuthorLabel.LabelProp = global::Mono.Unix.Catalog.GetString ("engine_author");
			this.hbox2.Add (this.EngineTwoAuthorLabel);
			global::Gtk.Box.BoxChild w22 = ((global::Gtk.Box.BoxChild)(this.hbox2 [this.EngineTwoAuthorLabel]));
			w22.Position = 1;
			// Container child hbox2.Gtk.Box+BoxChild
			this.EngineTwoDepthLabel = new global::Gtk.Label ();
			this.EngineTwoDepthLabel.Name = "EngineTwoDepthLabel";
			this.EngineTwoDepthLabel.Xpad = 10;
			this.EngineTwoDepthLabel.LabelProp = global::Mono.Unix.Catalog.GetString ("engine_depth");
			this.hbox2.Add (this.EngineTwoDepthLabel);
			global::Gtk.Box.BoxChild w23 = ((global::Gtk.Box.BoxChild)(this.hbox2 [this.EngineTwoDepthLabel]));
			w23.Position = 2;
			// Container child hbox2.Gtk.Box+BoxChild
			this.EngineTwoNPSLabel = new global::Gtk.Label ();
			this.EngineTwoNPSLabel.Name = "EngineTwoNPSLabel";
			this.EngineTwoNPSLabel.Xpad = 10;
			this.EngineTwoNPSLabel.LabelProp = global::Mono.Unix.Catalog.GetString ("engine_nps");
			this.hbox2.Add (this.EngineTwoNPSLabel);
			global::Gtk.Box.BoxChild w24 = ((global::Gtk.Box.BoxChild)(this.hbox2 [this.EngineTwoNPSLabel]));
			w24.Position = 3;
			this.vbox1.Add (this.hbox2);
			global::Gtk.Box.BoxChild w25 = ((global::Gtk.Box.BoxChild)(this.vbox1 [this.hbox2]));
			w25.Position = 3;
			w25.Expand = false;
			w25.Fill = false;
			w25.Padding = ((uint)(2));
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
			global::Gtk.Box.BoxChild w27 = ((global::Gtk.Box.BoxChild)(this.vbox1 [this.GtkScrolledWindow]));
			w27.Position = 4;
			this.Add (this.vbox1);
			if ((this.Child != null)) {
				this.Child.ShowAll ();
			}
			this.DefaultWidth = 773;
			this.DefaultHeight = 705;
			this.Show ();
			this.DeleteEvent += new global::Gtk.DeleteEventHandler (this.OnDeleteEvent);
			this.QuitAction.Activated += new global::System.EventHandler (this.OnQuit);
			this.LoadFENAction.Activated += new global::System.EventHandler (this.OnLoadFEN);
			this.LoadEngineOneAction.Activated += new global::System.EventHandler (this.OnLoadEngineOne);
			this.AboutAction.Activated += new global::System.EventHandler (this.OnAbout);
			this.ResetBoardAction1.Activated += new global::System.EventHandler (this.OnResetBoard);
			this.MakeEngineMoveAction.Activated += new global::System.EventHandler (this.OnMakeEngineMove);
			this.FlipBoardAction.Activated += new global::System.EventHandler (this.OnFlipBoard);
			this.SetClockAction.Activated += new global::System.EventHandler (this.OnSetClock);
			this.SetEngineStrengthAction.Activated += new global::System.EventHandler (this.OnSetEngineStrength);
			this.OnePlayerAction.Activated += new global::System.EventHandler (this.OnOnePlayerSet);
			this.TwoPlayerAction.Activated += new global::System.EventHandler (this.OnTwoPlayerSet);
			this.EnginesAction.Activated += new global::System.EventHandler (this.OnEnginesSet);
			this.AnalysePositionAction.Activated += new global::System.EventHandler (this.OnAnalyseMove);
			this.LoadEngine2Action.Activated += new global::System.EventHandler (this.OnLoadEngineTwo);
			this.BoardArea.ExposeEvent += new global::Gtk.ExposeEventHandler (this.OnBoardExpose);
			this.BoardArea.ButtonPressEvent += new global::Gtk.ButtonPressEventHandler (this.OnPieceClick);
		}
	}
}
