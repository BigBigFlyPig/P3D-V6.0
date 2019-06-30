namespace ObserverControl
{
    partial class ObserverControl
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.richResponse = new System.Windows.Forms.RichTextBox();
            this.buttonConnect = new System.Windows.Forms.Button();
            this.buttonDisconnect = new System.Windows.Forms.Button();
            this.buttonCreateObserver = new System.Windows.Forms.Button();
            this.textBoxCObserverName = new System.Windows.Forms.TextBox();
            this.textBoxCPosLat = new System.Windows.Forms.TextBox();
            this.textBoxCPosLong = new System.Windows.Forms.TextBox();
            this.textBoxCPosAlt = new System.Windows.Forms.TextBox();
            this.textBoxCHeading = new System.Windows.Forms.TextBox();
            this.textBoxCPitch = new System.Windows.Forms.TextBox();
            this.textBoxCBank = new System.Windows.Forms.TextBox();
            this.textBoxCFOVH = new System.Windows.Forms.TextBox();
            this.textBoxCFOVV = new System.Windows.Forms.TextBox();
            this.textBoxCSSLinear = new System.Windows.Forms.TextBox();
            this.textBoxCSSAngular = new System.Windows.Forms.TextBox();
            this.textBoxCFocalLength = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.radioButtonCFocusWorld = new System.Windows.Forms.RadioButton();
            this.radioButtonCFocusPoint = new System.Windows.Forms.RadioButton();
            this.panel1 = new System.Windows.Forms.Panel();
            this.checkBoxCPassWater = new System.Windows.Forms.CheckBox();
            this.checkBoxCGhostMode = new System.Windows.Forms.CheckBox();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.buttonPopulateFields = new System.Windows.Forms.Button();
            this.buttonSetRegime = new System.Windows.Forms.Button();
            this.buttonSetFocus = new System.Windows.Forms.Button();
            this.buttonSetStepSize = new System.Windows.Forms.Button();
            this.buttonSetFieldOfView = new System.Windows.Forms.Button();
            this.buttonSetRotation = new System.Windows.Forms.Button();
            this.buttonSetPosition = new System.Windows.Forms.Button();
            this.label13 = new System.Windows.Forms.Label();
            this.panel2 = new System.Windows.Forms.Panel();
            this.radioButtonMFocusPoint = new System.Windows.Forms.RadioButton();
            this.radioButtonMFocusWorld = new System.Windows.Forms.RadioButton();
            this.label14 = new System.Windows.Forms.Label();
            this.textBoxMFocalLength = new System.Windows.Forms.TextBox();
            this.textBoxMObserverName = new System.Windows.Forms.TextBox();
            this.textBoxMPosLat = new System.Windows.Forms.TextBox();
            this.textBoxMPosLong = new System.Windows.Forms.TextBox();
            this.textBoxMPosAlt = new System.Windows.Forms.TextBox();
            this.checkBoxMGhostMode = new System.Windows.Forms.CheckBox();
            this.textBoxMHeading = new System.Windows.Forms.TextBox();
            this.checkBoxMPassWater = new System.Windows.Forms.CheckBox();
            this.textBoxMPitch = new System.Windows.Forms.TextBox();
            this.textBoxMBank = new System.Windows.Forms.TextBox();
            this.label15 = new System.Windows.Forms.Label();
            this.textBoxMFOVH = new System.Windows.Forms.TextBox();
            this.label16 = new System.Windows.Forms.Label();
            this.textBoxMFOVV = new System.Windows.Forms.TextBox();
            this.label17 = new System.Windows.Forms.Label();
            this.textBoxMSSLinear = new System.Windows.Forms.TextBox();
            this.label18 = new System.Windows.Forms.Label();
            this.textBoxMSSAngular = new System.Windows.Forms.TextBox();
            this.label19 = new System.Windows.Forms.Label();
            this.label20 = new System.Windows.Forms.Label();
            this.label21 = new System.Windows.Forms.Label();
            this.label22 = new System.Windows.Forms.Label();
            this.label23 = new System.Windows.Forms.Label();
            this.label24 = new System.Windows.Forms.Label();
            this.panel1.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.panel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // richResponse
            // 
            this.richResponse.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.richResponse.ForeColor = System.Drawing.SystemColors.WindowText;
            this.richResponse.Location = new System.Drawing.Point(14, 44);
            this.richResponse.Name = "richResponse";
            this.richResponse.ReadOnly = true;
            this.richResponse.Size = new System.Drawing.Size(456, 72);
            this.richResponse.TabIndex = 4;
            this.richResponse.Text = "";
            // 
            // buttonConnect
            // 
            this.buttonConnect.Location = new System.Drawing.Point(12, 12);
            this.buttonConnect.Name = "buttonConnect";
            this.buttonConnect.Size = new System.Drawing.Size(131, 26);
            this.buttonConnect.TabIndex = 5;
            this.buttonConnect.Text = "Connect to Prepar3D";
            this.buttonConnect.UseVisualStyleBackColor = true;
            this.buttonConnect.Click += new System.EventHandler(this.buttonConnect_Click);
            // 
            // buttonDisconnect
            // 
            this.buttonDisconnect.Location = new System.Drawing.Point(339, 14);
            this.buttonDisconnect.Name = "buttonDisconnect";
            this.buttonDisconnect.Size = new System.Drawing.Size(131, 23);
            this.buttonDisconnect.TabIndex = 6;
            this.buttonDisconnect.Text = "Disconnect from P3D";
            this.buttonDisconnect.UseVisualStyleBackColor = true;
            this.buttonDisconnect.Click += new System.EventHandler(this.buttonDisconnect_Click);
            // 
            // buttonCreateObserver
            // 
            this.buttonCreateObserver.Location = new System.Drawing.Point(12, 341);
            this.buttonCreateObserver.Name = "buttonCreateObserver";
            this.buttonCreateObserver.Size = new System.Drawing.Size(130, 23);
            this.buttonCreateObserver.TabIndex = 7;
            this.buttonCreateObserver.Text = "Create Observer";
            this.buttonCreateObserver.UseVisualStyleBackColor = true;
            this.buttonCreateObserver.Click += new System.EventHandler(this.buttonCreateObserver_Click);
            // 
            // textBoxCObserverName
            // 
            this.textBoxCObserverName.Location = new System.Drawing.Point(12, 27);
            this.textBoxCObserverName.Name = "textBoxCObserverName";
            this.textBoxCObserverName.Size = new System.Drawing.Size(315, 20);
            this.textBoxCObserverName.TabIndex = 8;
            // 
            // textBoxCPosLat
            // 
            this.textBoxCPosLat.Location = new System.Drawing.Point(12, 76);
            this.textBoxCPosLat.Name = "textBoxCPosLat";
            this.textBoxCPosLat.Size = new System.Drawing.Size(100, 20);
            this.textBoxCPosLat.TabIndex = 9;
            // 
            // textBoxCPosLong
            // 
            this.textBoxCPosLong.Location = new System.Drawing.Point(121, 76);
            this.textBoxCPosLong.Name = "textBoxCPosLong";
            this.textBoxCPosLong.Size = new System.Drawing.Size(100, 20);
            this.textBoxCPosLong.TabIndex = 10;
            // 
            // textBoxCPosAlt
            // 
            this.textBoxCPosAlt.Location = new System.Drawing.Point(227, 76);
            this.textBoxCPosAlt.Name = "textBoxCPosAlt";
            this.textBoxCPosAlt.Size = new System.Drawing.Size(100, 20);
            this.textBoxCPosAlt.TabIndex = 11;
            // 
            // textBoxCHeading
            // 
            this.textBoxCHeading.Location = new System.Drawing.Point(12, 122);
            this.textBoxCHeading.Name = "textBoxCHeading";
            this.textBoxCHeading.Size = new System.Drawing.Size(100, 20);
            this.textBoxCHeading.TabIndex = 12;
            // 
            // textBoxCPitch
            // 
            this.textBoxCPitch.Location = new System.Drawing.Point(121, 122);
            this.textBoxCPitch.Name = "textBoxCPitch";
            this.textBoxCPitch.Size = new System.Drawing.Size(100, 20);
            this.textBoxCPitch.TabIndex = 13;
            // 
            // textBoxCBank
            // 
            this.textBoxCBank.Location = new System.Drawing.Point(227, 122);
            this.textBoxCBank.Name = "textBoxCBank";
            this.textBoxCBank.Size = new System.Drawing.Size(100, 20);
            this.textBoxCBank.TabIndex = 14;
            // 
            // textBoxCFOVH
            // 
            this.textBoxCFOVH.Location = new System.Drawing.Point(12, 169);
            this.textBoxCFOVH.Name = "textBoxCFOVH";
            this.textBoxCFOVH.Size = new System.Drawing.Size(100, 20);
            this.textBoxCFOVH.TabIndex = 15;
            // 
            // textBoxCFOVV
            // 
            this.textBoxCFOVV.Location = new System.Drawing.Point(121, 169);
            this.textBoxCFOVV.Name = "textBoxCFOVV";
            this.textBoxCFOVV.Size = new System.Drawing.Size(100, 20);
            this.textBoxCFOVV.TabIndex = 16;
            // 
            // textBoxCSSLinear
            // 
            this.textBoxCSSLinear.Location = new System.Drawing.Point(12, 216);
            this.textBoxCSSLinear.Name = "textBoxCSSLinear";
            this.textBoxCSSLinear.Size = new System.Drawing.Size(100, 20);
            this.textBoxCSSLinear.TabIndex = 17;
            // 
            // textBoxCSSAngular
            // 
            this.textBoxCSSAngular.Location = new System.Drawing.Point(121, 216);
            this.textBoxCSSAngular.Name = "textBoxCSSAngular";
            this.textBoxCSSAngular.Size = new System.Drawing.Size(100, 20);
            this.textBoxCSSAngular.TabIndex = 18;
            // 
            // textBoxCFocalLength
            // 
            this.textBoxCFocalLength.Location = new System.Drawing.Point(6, 21);
            this.textBoxCFocalLength.Name = "textBoxCFocalLength";
            this.textBoxCFocalLength.Size = new System.Drawing.Size(100, 20);
            this.textBoxCFocalLength.TabIndex = 19;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 11);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(81, 13);
            this.label1.TabIndex = 20;
            this.label1.Text = "Observer Name";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 60);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(45, 13);
            this.label2.TabIndex = 21;
            this.label2.Text = "Latitude";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(118, 60);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(54, 13);
            this.label3.TabIndex = 22;
            this.label3.Text = "Longitude";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(227, 60);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(42, 13);
            this.label4.TabIndex = 23;
            this.label4.Text = "Altitude";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(12, 106);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(47, 13);
            this.label5.TabIndex = 24;
            this.label5.Text = "Heading";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(118, 106);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(31, 13);
            this.label6.TabIndex = 25;
            this.label6.Text = "Pitch";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(227, 106);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(32, 13);
            this.label7.TabIndex = 26;
            this.label7.Text = "Bank";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(12, 153);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(86, 13);
            this.label8.TabIndex = 27;
            this.label8.Text = "Field-Of-View (H)";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(118, 153);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(85, 13);
            this.label9.TabIndex = 28;
            this.label9.Text = "Field-Of-View (V)";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(12, 200);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(84, 13);
            this.label10.TabIndex = 29;
            this.label10.Text = "Linear Step Size";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(118, 200);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(91, 13);
            this.label11.TabIndex = 30;
            this.label11.Text = "Angular Step Size";
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(6, 5);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(69, 13);
            this.label12.TabIndex = 31;
            this.label12.Text = "Focal Length";
            // 
            // radioButtonCFocusWorld
            // 
            this.radioButtonCFocusWorld.AutoSize = true;
            this.radioButtonCFocusWorld.Location = new System.Drawing.Point(203, 22);
            this.radioButtonCFocusWorld.Name = "radioButtonCFocusWorld";
            this.radioButtonCFocusWorld.Size = new System.Drawing.Size(85, 17);
            this.radioButtonCFocusWorld.TabIndex = 33;
            this.radioButtonCFocusWorld.TabStop = true;
            this.radioButtonCFocusWorld.Text = "World Focus";
            this.radioButtonCFocusWorld.UseVisualStyleBackColor = true;
            // 
            // radioButtonCFocusPoint
            // 
            this.radioButtonCFocusPoint.AutoSize = true;
            this.radioButtonCFocusPoint.Location = new System.Drawing.Point(116, 22);
            this.radioButtonCFocusPoint.Name = "radioButtonCFocusPoint";
            this.radioButtonCFocusPoint.Size = new System.Drawing.Size(81, 17);
            this.radioButtonCFocusPoint.TabIndex = 32;
            this.radioButtonCFocusPoint.TabStop = true;
            this.radioButtonCFocusPoint.Text = "Point Focus";
            this.radioButtonCFocusPoint.UseVisualStyleBackColor = true;
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.radioButtonCFocusPoint);
            this.panel1.Controls.Add(this.radioButtonCFocusWorld);
            this.panel1.Controls.Add(this.label12);
            this.panel1.Controls.Add(this.textBoxCFocalLength);
            this.panel1.Location = new System.Drawing.Point(6, 242);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(307, 51);
            this.panel1.TabIndex = 36;
            // 
            // checkBoxCPassWater
            // 
            this.checkBoxCPassWater.AutoSize = true;
            this.checkBoxCPassWater.Location = new System.Drawing.Point(15, 312);
            this.checkBoxCPassWater.Name = "checkBoxCPassWater";
            this.checkBoxCPassWater.Size = new System.Drawing.Size(169, 17);
            this.checkBoxCPassWater.TabIndex = 37;
            this.checkBoxCPassWater.Text = "Pass Through Water Surfaces";
            this.checkBoxCPassWater.UseVisualStyleBackColor = true;
            // 
            // checkBoxCGhostMode
            // 
            this.checkBoxCGhostMode.AutoSize = true;
            this.checkBoxCGhostMode.Location = new System.Drawing.Point(190, 312);
            this.checkBoxCGhostMode.Name = "checkBoxCGhostMode";
            this.checkBoxCGhostMode.Size = new System.Drawing.Size(84, 17);
            this.checkBoxCGhostMode.TabIndex = 38;
            this.checkBoxCGhostMode.Text = "Ghost Mode";
            this.checkBoxCGhostMode.UseVisualStyleBackColor = true;
            this.checkBoxCGhostMode.CheckedChanged += new System.EventHandler(this.checkBoxCGhostMode_CheckedChanged);
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Location = new System.Drawing.Point(14, 131);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(456, 401);
            this.tabControl1.TabIndex = 45;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.label1);
            this.tabPage1.Controls.Add(this.panel1);
            this.tabPage1.Controls.Add(this.buttonCreateObserver);
            this.tabPage1.Controls.Add(this.textBoxCObserverName);
            this.tabPage1.Controls.Add(this.textBoxCPosLat);
            this.tabPage1.Controls.Add(this.textBoxCPosLong);
            this.tabPage1.Controls.Add(this.textBoxCPosAlt);
            this.tabPage1.Controls.Add(this.checkBoxCGhostMode);
            this.tabPage1.Controls.Add(this.textBoxCHeading);
            this.tabPage1.Controls.Add(this.checkBoxCPassWater);
            this.tabPage1.Controls.Add(this.textBoxCPitch);
            this.tabPage1.Controls.Add(this.textBoxCBank);
            this.tabPage1.Controls.Add(this.label11);
            this.tabPage1.Controls.Add(this.textBoxCFOVH);
            this.tabPage1.Controls.Add(this.label10);
            this.tabPage1.Controls.Add(this.textBoxCFOVV);
            this.tabPage1.Controls.Add(this.label9);
            this.tabPage1.Controls.Add(this.textBoxCSSLinear);
            this.tabPage1.Controls.Add(this.label8);
            this.tabPage1.Controls.Add(this.textBoxCSSAngular);
            this.tabPage1.Controls.Add(this.label7);
            this.tabPage1.Controls.Add(this.label6);
            this.tabPage1.Controls.Add(this.label2);
            this.tabPage1.Controls.Add(this.label5);
            this.tabPage1.Controls.Add(this.label3);
            this.tabPage1.Controls.Add(this.label4);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(448, 375);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Create Observer";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.buttonPopulateFields);
            this.tabPage2.Controls.Add(this.buttonSetRegime);
            this.tabPage2.Controls.Add(this.buttonSetFocus);
            this.tabPage2.Controls.Add(this.buttonSetStepSize);
            this.tabPage2.Controls.Add(this.buttonSetFieldOfView);
            this.tabPage2.Controls.Add(this.buttonSetRotation);
            this.tabPage2.Controls.Add(this.buttonSetPosition);
            this.tabPage2.Controls.Add(this.label13);
            this.tabPage2.Controls.Add(this.panel2);
            this.tabPage2.Controls.Add(this.textBoxMObserverName);
            this.tabPage2.Controls.Add(this.textBoxMPosLat);
            this.tabPage2.Controls.Add(this.textBoxMPosLong);
            this.tabPage2.Controls.Add(this.textBoxMPosAlt);
            this.tabPage2.Controls.Add(this.checkBoxMGhostMode);
            this.tabPage2.Controls.Add(this.textBoxMHeading);
            this.tabPage2.Controls.Add(this.checkBoxMPassWater);
            this.tabPage2.Controls.Add(this.textBoxMPitch);
            this.tabPage2.Controls.Add(this.textBoxMBank);
            this.tabPage2.Controls.Add(this.label15);
            this.tabPage2.Controls.Add(this.textBoxMFOVH);
            this.tabPage2.Controls.Add(this.label16);
            this.tabPage2.Controls.Add(this.textBoxMFOVV);
            this.tabPage2.Controls.Add(this.label17);
            this.tabPage2.Controls.Add(this.textBoxMSSLinear);
            this.tabPage2.Controls.Add(this.label18);
            this.tabPage2.Controls.Add(this.textBoxMSSAngular);
            this.tabPage2.Controls.Add(this.label19);
            this.tabPage2.Controls.Add(this.label20);
            this.tabPage2.Controls.Add(this.label21);
            this.tabPage2.Controls.Add(this.label22);
            this.tabPage2.Controls.Add(this.label23);
            this.tabPage2.Controls.Add(this.label24);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(448, 375);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Modify Observer";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // buttonPopulateFields
            // 
            this.buttonPopulateFields.Location = new System.Drawing.Point(333, 27);
            this.buttonPopulateFields.Name = "buttonPopulateFields";
            this.buttonPopulateFields.Size = new System.Drawing.Size(103, 23);
            this.buttonPopulateFields.TabIndex = 77;
            this.buttonPopulateFields.Text = "Populate Fields";
            this.buttonPopulateFields.UseVisualStyleBackColor = true;
            this.buttonPopulateFields.Click += new System.EventHandler(this.buttonPopulateFields_Click);
            // 
            // buttonSetRegime
            // 
            this.buttonSetRegime.Location = new System.Drawing.Point(333, 308);
            this.buttonSetRegime.Name = "buttonSetRegime";
            this.buttonSetRegime.Size = new System.Drawing.Size(103, 23);
            this.buttonSetRegime.TabIndex = 76;
            this.buttonSetRegime.Text = "Set Regime";
            this.buttonSetRegime.UseVisualStyleBackColor = true;
            this.buttonSetRegime.Click += new System.EventHandler(this.buttonSetRegime_Click);
            // 
            // buttonSetFocus
            // 
            this.buttonSetFocus.Location = new System.Drawing.Point(333, 261);
            this.buttonSetFocus.Name = "buttonSetFocus";
            this.buttonSetFocus.Size = new System.Drawing.Size(103, 23);
            this.buttonSetFocus.TabIndex = 75;
            this.buttonSetFocus.Text = "Set Focus";
            this.buttonSetFocus.UseVisualStyleBackColor = true;
            this.buttonSetFocus.Click += new System.EventHandler(this.buttonSetFocus_Click);
            // 
            // buttonSetStepSize
            // 
            this.buttonSetStepSize.Location = new System.Drawing.Point(333, 214);
            this.buttonSetStepSize.Name = "buttonSetStepSize";
            this.buttonSetStepSize.Size = new System.Drawing.Size(103, 23);
            this.buttonSetStepSize.TabIndex = 74;
            this.buttonSetStepSize.Text = "Set Step Size";
            this.buttonSetStepSize.UseVisualStyleBackColor = true;
            this.buttonSetStepSize.Click += new System.EventHandler(this.buttonSetStepSize_Click);
            // 
            // buttonSetFieldOfView
            // 
            this.buttonSetFieldOfView.Location = new System.Drawing.Point(333, 167);
            this.buttonSetFieldOfView.Name = "buttonSetFieldOfView";
            this.buttonSetFieldOfView.Size = new System.Drawing.Size(103, 23);
            this.buttonSetFieldOfView.TabIndex = 73;
            this.buttonSetFieldOfView.Text = "Set Field-Of-View";
            this.buttonSetFieldOfView.UseVisualStyleBackColor = true;
            this.buttonSetFieldOfView.Click += new System.EventHandler(this.buttonSetFieldOfView_Click);
            // 
            // buttonSetRotation
            // 
            this.buttonSetRotation.Location = new System.Drawing.Point(333, 120);
            this.buttonSetRotation.Name = "buttonSetRotation";
            this.buttonSetRotation.Size = new System.Drawing.Size(103, 23);
            this.buttonSetRotation.TabIndex = 72;
            this.buttonSetRotation.Text = "Set Rotation";
            this.buttonSetRotation.UseVisualStyleBackColor = true;
            this.buttonSetRotation.Click += new System.EventHandler(this.buttonSetRotation_Click);
            // 
            // buttonSetPosition
            // 
            this.buttonSetPosition.Location = new System.Drawing.Point(333, 73);
            this.buttonSetPosition.Name = "buttonSetPosition";
            this.buttonSetPosition.Size = new System.Drawing.Size(103, 23);
            this.buttonSetPosition.TabIndex = 71;
            this.buttonSetPosition.Text = "Set Position";
            this.buttonSetPosition.UseVisualStyleBackColor = true;
            this.buttonSetPosition.Click += new System.EventHandler(this.buttonSetPosition_Click);
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(12, 11);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(81, 13);
            this.label13.TabIndex = 57;
            this.label13.Text = "Observer Name";
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.radioButtonMFocusPoint);
            this.panel2.Controls.Add(this.radioButtonMFocusWorld);
            this.panel2.Controls.Add(this.label14);
            this.panel2.Controls.Add(this.textBoxMFocalLength);
            this.panel2.Location = new System.Drawing.Point(6, 242);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(307, 51);
            this.panel2.TabIndex = 68;
            // 
            // radioButtonMFocusPoint
            // 
            this.radioButtonMFocusPoint.AutoSize = true;
            this.radioButtonMFocusPoint.Location = new System.Drawing.Point(116, 22);
            this.radioButtonMFocusPoint.Name = "radioButtonMFocusPoint";
            this.radioButtonMFocusPoint.Size = new System.Drawing.Size(81, 17);
            this.radioButtonMFocusPoint.TabIndex = 32;
            this.radioButtonMFocusPoint.TabStop = true;
            this.radioButtonMFocusPoint.Text = "Point Focus";
            this.radioButtonMFocusPoint.UseVisualStyleBackColor = true;
            // 
            // radioButtonMFocusWorld
            // 
            this.radioButtonMFocusWorld.AutoSize = true;
            this.radioButtonMFocusWorld.Location = new System.Drawing.Point(203, 22);
            this.radioButtonMFocusWorld.Name = "radioButtonMFocusWorld";
            this.radioButtonMFocusWorld.Size = new System.Drawing.Size(85, 17);
            this.radioButtonMFocusWorld.TabIndex = 33;
            this.radioButtonMFocusWorld.TabStop = true;
            this.radioButtonMFocusWorld.Text = "World Focus";
            this.radioButtonMFocusWorld.UseVisualStyleBackColor = true;
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(6, 5);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(69, 13);
            this.label14.TabIndex = 31;
            this.label14.Text = "Focal Length";
            // 
            // textBoxMFocalLength
            // 
            this.textBoxMFocalLength.Location = new System.Drawing.Point(6, 21);
            this.textBoxMFocalLength.Name = "textBoxMFocalLength";
            this.textBoxMFocalLength.Size = new System.Drawing.Size(100, 20);
            this.textBoxMFocalLength.TabIndex = 19;
            // 
            // textBoxMObserverName
            // 
            this.textBoxMObserverName.Location = new System.Drawing.Point(12, 27);
            this.textBoxMObserverName.Name = "textBoxMObserverName";
            this.textBoxMObserverName.Size = new System.Drawing.Size(315, 20);
            this.textBoxMObserverName.TabIndex = 46;
            // 
            // textBoxMPosLat
            // 
            this.textBoxMPosLat.Location = new System.Drawing.Point(12, 76);
            this.textBoxMPosLat.Name = "textBoxMPosLat";
            this.textBoxMPosLat.Size = new System.Drawing.Size(100, 20);
            this.textBoxMPosLat.TabIndex = 47;
            // 
            // textBoxMPosLong
            // 
            this.textBoxMPosLong.Location = new System.Drawing.Point(121, 76);
            this.textBoxMPosLong.Name = "textBoxMPosLong";
            this.textBoxMPosLong.Size = new System.Drawing.Size(100, 20);
            this.textBoxMPosLong.TabIndex = 48;
            // 
            // textBoxMPosAlt
            // 
            this.textBoxMPosAlt.Location = new System.Drawing.Point(227, 76);
            this.textBoxMPosAlt.Name = "textBoxMPosAlt";
            this.textBoxMPosAlt.Size = new System.Drawing.Size(100, 20);
            this.textBoxMPosAlt.TabIndex = 49;
            // 
            // checkBoxMGhostMode
            // 
            this.checkBoxMGhostMode.AutoSize = true;
            this.checkBoxMGhostMode.Location = new System.Drawing.Point(190, 312);
            this.checkBoxMGhostMode.Name = "checkBoxMGhostMode";
            this.checkBoxMGhostMode.Size = new System.Drawing.Size(84, 17);
            this.checkBoxMGhostMode.TabIndex = 70;
            this.checkBoxMGhostMode.Text = "Ghost Mode";
            this.checkBoxMGhostMode.UseVisualStyleBackColor = true;
            this.checkBoxMGhostMode.CheckedChanged += new System.EventHandler(this.checkBoxMGhostMode_CheckedChanged);
            // 
            // textBoxMHeading
            // 
            this.textBoxMHeading.Location = new System.Drawing.Point(12, 122);
            this.textBoxMHeading.Name = "textBoxMHeading";
            this.textBoxMHeading.Size = new System.Drawing.Size(100, 20);
            this.textBoxMHeading.TabIndex = 50;
            // 
            // checkBoxMPassWater
            // 
            this.checkBoxMPassWater.AutoSize = true;
            this.checkBoxMPassWater.Location = new System.Drawing.Point(15, 312);
            this.checkBoxMPassWater.Name = "checkBoxMPassWater";
            this.checkBoxMPassWater.Size = new System.Drawing.Size(169, 17);
            this.checkBoxMPassWater.TabIndex = 69;
            this.checkBoxMPassWater.Text = "Pass Through Water Surfaces";
            this.checkBoxMPassWater.UseVisualStyleBackColor = true;
            // 
            // textBoxMPitch
            // 
            this.textBoxMPitch.Location = new System.Drawing.Point(121, 122);
            this.textBoxMPitch.Name = "textBoxMPitch";
            this.textBoxMPitch.Size = new System.Drawing.Size(100, 20);
            this.textBoxMPitch.TabIndex = 51;
            // 
            // textBoxMBank
            // 
            this.textBoxMBank.Location = new System.Drawing.Point(227, 122);
            this.textBoxMBank.Name = "textBoxMBank";
            this.textBoxMBank.Size = new System.Drawing.Size(100, 20);
            this.textBoxMBank.TabIndex = 52;
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(118, 200);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(91, 13);
            this.label15.TabIndex = 67;
            this.label15.Text = "Angular Step Size";
            // 
            // textBoxMFOVH
            // 
            this.textBoxMFOVH.Location = new System.Drawing.Point(12, 169);
            this.textBoxMFOVH.Name = "textBoxMFOVH";
            this.textBoxMFOVH.Size = new System.Drawing.Size(100, 20);
            this.textBoxMFOVH.TabIndex = 53;
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Location = new System.Drawing.Point(12, 200);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(84, 13);
            this.label16.TabIndex = 66;
            this.label16.Text = "Linear Step Size";
            // 
            // textBoxMFOVV
            // 
            this.textBoxMFOVV.Location = new System.Drawing.Point(121, 169);
            this.textBoxMFOVV.Name = "textBoxMFOVV";
            this.textBoxMFOVV.Size = new System.Drawing.Size(100, 20);
            this.textBoxMFOVV.TabIndex = 54;
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Location = new System.Drawing.Point(118, 153);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(85, 13);
            this.label17.TabIndex = 65;
            this.label17.Text = "Field-Of-View (V)";
            // 
            // textBoxMSSLinear
            // 
            this.textBoxMSSLinear.Location = new System.Drawing.Point(12, 216);
            this.textBoxMSSLinear.Name = "textBoxMSSLinear";
            this.textBoxMSSLinear.Size = new System.Drawing.Size(100, 20);
            this.textBoxMSSLinear.TabIndex = 55;
            // 
            // label18
            // 
            this.label18.AutoSize = true;
            this.label18.Location = new System.Drawing.Point(12, 153);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(86, 13);
            this.label18.TabIndex = 64;
            this.label18.Text = "Field-Of-View (H)";
            // 
            // textBoxMSSAngular
            // 
            this.textBoxMSSAngular.Location = new System.Drawing.Point(121, 216);
            this.textBoxMSSAngular.Name = "textBoxMSSAngular";
            this.textBoxMSSAngular.Size = new System.Drawing.Size(100, 20);
            this.textBoxMSSAngular.TabIndex = 56;
            // 
            // label19
            // 
            this.label19.AutoSize = true;
            this.label19.Location = new System.Drawing.Point(227, 106);
            this.label19.Name = "label19";
            this.label19.Size = new System.Drawing.Size(32, 13);
            this.label19.TabIndex = 63;
            this.label19.Text = "Bank";
            // 
            // label20
            // 
            this.label20.AutoSize = true;
            this.label20.Location = new System.Drawing.Point(118, 106);
            this.label20.Name = "label20";
            this.label20.Size = new System.Drawing.Size(31, 13);
            this.label20.TabIndex = 62;
            this.label20.Text = "Pitch";
            // 
            // label21
            // 
            this.label21.AutoSize = true;
            this.label21.Location = new System.Drawing.Point(12, 60);
            this.label21.Name = "label21";
            this.label21.Size = new System.Drawing.Size(45, 13);
            this.label21.TabIndex = 58;
            this.label21.Text = "Latitude";
            // 
            // label22
            // 
            this.label22.AutoSize = true;
            this.label22.Location = new System.Drawing.Point(12, 106);
            this.label22.Name = "label22";
            this.label22.Size = new System.Drawing.Size(47, 13);
            this.label22.TabIndex = 61;
            this.label22.Text = "Heading";
            // 
            // label23
            // 
            this.label23.AutoSize = true;
            this.label23.Location = new System.Drawing.Point(118, 60);
            this.label23.Name = "label23";
            this.label23.Size = new System.Drawing.Size(54, 13);
            this.label23.TabIndex = 59;
            this.label23.Text = "Longitude";
            // 
            // label24
            // 
            this.label24.AutoSize = true;
            this.label24.Location = new System.Drawing.Point(227, 60);
            this.label24.Name = "label24";
            this.label24.Size = new System.Drawing.Size(42, 13);
            this.label24.TabIndex = 60;
            this.label24.Text = "Altitude";
            // 
            // ObserverControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(484, 544);
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.buttonDisconnect);
            this.Controls.Add(this.buttonConnect);
            this.Controls.Add(this.richResponse);
            this.Name = "ObserverControl";
            this.Text = "Observer Control";
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.RichTextBox richResponse;
        private System.Windows.Forms.Button buttonConnect;
        private System.Windows.Forms.Button buttonDisconnect;
        private System.Windows.Forms.Button buttonCreateObserver;
        private System.Windows.Forms.TextBox textBoxCObserverName;
        private System.Windows.Forms.TextBox textBoxCPosLat;
        private System.Windows.Forms.TextBox textBoxCPosLong;
        private System.Windows.Forms.TextBox textBoxCPosAlt;
        private System.Windows.Forms.TextBox textBoxCHeading;
        private System.Windows.Forms.TextBox textBoxCPitch;
        private System.Windows.Forms.TextBox textBoxCBank;
        private System.Windows.Forms.TextBox textBoxCFOVH;
        private System.Windows.Forms.TextBox textBoxCFOVV;
        private System.Windows.Forms.TextBox textBoxCSSLinear;
        private System.Windows.Forms.TextBox textBoxCSSAngular;
        private System.Windows.Forms.TextBox textBoxCFocalLength;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.RadioButton radioButtonCFocusWorld;
        private System.Windows.Forms.RadioButton radioButtonCFocusPoint;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.CheckBox checkBoxCPassWater;
        private System.Windows.Forms.CheckBox checkBoxCGhostMode;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.RadioButton radioButtonMFocusPoint;
        private System.Windows.Forms.RadioButton radioButtonMFocusWorld;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.TextBox textBoxMFocalLength;
        private System.Windows.Forms.TextBox textBoxMObserverName;
        private System.Windows.Forms.TextBox textBoxMPosLat;
        private System.Windows.Forms.TextBox textBoxMPosLong;
        private System.Windows.Forms.TextBox textBoxMPosAlt;
        private System.Windows.Forms.CheckBox checkBoxMGhostMode;
        private System.Windows.Forms.TextBox textBoxMHeading;
        private System.Windows.Forms.CheckBox checkBoxMPassWater;
        private System.Windows.Forms.TextBox textBoxMPitch;
        private System.Windows.Forms.TextBox textBoxMBank;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.TextBox textBoxMFOVH;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.TextBox textBoxMFOVV;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.TextBox textBoxMSSLinear;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.TextBox textBoxMSSAngular;
        private System.Windows.Forms.Label label19;
        private System.Windows.Forms.Label label20;
        private System.Windows.Forms.Label label21;
        private System.Windows.Forms.Label label22;
        private System.Windows.Forms.Label label23;
        private System.Windows.Forms.Label label24;
        private System.Windows.Forms.Button buttonSetRegime;
        private System.Windows.Forms.Button buttonSetFocus;
        private System.Windows.Forms.Button buttonSetStepSize;
        private System.Windows.Forms.Button buttonSetFieldOfView;
        private System.Windows.Forms.Button buttonSetRotation;
        private System.Windows.Forms.Button buttonSetPosition;
        private System.Windows.Forms.Button buttonPopulateFields;
    }
}

