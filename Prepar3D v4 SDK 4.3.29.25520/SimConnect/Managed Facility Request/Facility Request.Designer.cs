//Copyright (c) Lockheed Martin Corporation.  All rights reserved. 
namespace Managed_Facility_Request
{
    partial class Form1
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.buttonConnect = new System.Windows.Forms.Button();
            this.buttonDisconnect = new System.Windows.Forms.Button();
            this.buttonRequestAirports = new System.Windows.Forms.Button();
            this.richResponse = new System.Windows.Forms.RichTextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.buttonRequestWaypoints = new System.Windows.Forms.Button();
            this.buttonRequestVORs = new System.Windows.Forms.Button();
            this.buttonRequestNDBs = new System.Windows.Forms.Button();
            this.labelSubscriptions = new System.Windows.Forms.Label();
            this.checkboxAirportSubscription = new System.Windows.Forms.CheckBox();
            this.checkboxWaypointsSubscription = new System.Windows.Forms.CheckBox();
            this.checkboxNDBsSubscription = new System.Windows.Forms.CheckBox();
            this.checkboxVORsSubscription = new System.Windows.Forms.CheckBox();
            this.buttonRequestTacans = new System.Windows.Forms.Button();
            this.checkboxTacansSubscription = new System.Windows.Forms.CheckBox();
            this.SuspendLayout();
            // 
            // buttonConnect
            // 
            this.buttonConnect.Location = new System.Drawing.Point(12, 25);
            this.buttonConnect.Name = "buttonConnect";
            this.buttonConnect.Size = new System.Drawing.Size(131, 23);
            this.buttonConnect.TabIndex = 0;
            this.buttonConnect.Text = "Connect to P3D";
            this.buttonConnect.UseVisualStyleBackColor = true;
            this.buttonConnect.Click += new System.EventHandler(this.buttonConnect_Click);
            // 
            // buttonDisconnect
            // 
            this.buttonDisconnect.Location = new System.Drawing.Point(12, 400);
            this.buttonDisconnect.Name = "buttonDisconnect";
            this.buttonDisconnect.Size = new System.Drawing.Size(131, 23);
            this.buttonDisconnect.TabIndex = 1;
            this.buttonDisconnect.Text = "Disconnect from P3D";
            this.buttonDisconnect.UseVisualStyleBackColor = true;
            this.buttonDisconnect.Click += new System.EventHandler(this.buttonDisconnect_Click);
            // 
            // buttonRequestAirports
            // 
            this.buttonRequestAirports.Location = new System.Drawing.Point(12, 87);
            this.buttonRequestAirports.Name = "buttonRequestAirports";
            this.buttonRequestAirports.Size = new System.Drawing.Size(131, 23);
            this.buttonRequestAirports.TabIndex = 2;
            this.buttonRequestAirports.Text = "Request Airports";
            this.buttonRequestAirports.UseVisualStyleBackColor = true;
            this.buttonRequestAirports.Click += new System.EventHandler(this.buttonRequestAirports_Click);
            // 
            // richResponse
            // 
            this.richResponse.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.richResponse.Location = new System.Drawing.Point(149, 25);
            this.richResponse.Name = "richResponse";
            this.richResponse.ReadOnly = true;
            this.richResponse.Size = new System.Drawing.Size(746, 412);
            this.richResponse.TabIndex = 3;
            this.richResponse.Text = "";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(146, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(60, 13);
            this.label1.TabIndex = 4;
            this.label1.Text = "Responses";
            // 
            // buttonRequestWaypoints
            // 
            this.buttonRequestWaypoints.Location = new System.Drawing.Point(12, 116);
            this.buttonRequestWaypoints.Name = "buttonRequestWaypoints";
            this.buttonRequestWaypoints.Size = new System.Drawing.Size(131, 23);
            this.buttonRequestWaypoints.TabIndex = 5;
            this.buttonRequestWaypoints.Text = "Request Waypoints";
            this.buttonRequestWaypoints.UseVisualStyleBackColor = true;
            this.buttonRequestWaypoints.Click += new System.EventHandler(this.buttonRequestWaypoints_Click);
            // 
            // buttonRequestVORs
            // 
            this.buttonRequestVORs.Location = new System.Drawing.Point(12, 175);
            this.buttonRequestVORs.Name = "buttonRequestVORs";
            this.buttonRequestVORs.Size = new System.Drawing.Size(131, 23);
            this.buttonRequestVORs.TabIndex = 6;
            this.buttonRequestVORs.Text = "Request VORs";
            this.buttonRequestVORs.UseVisualStyleBackColor = true;
            this.buttonRequestVORs.Click += new System.EventHandler(this.buttonRequestVORs_Click);
            // 
            // buttonRequestNDBs
            // 
            this.buttonRequestNDBs.Location = new System.Drawing.Point(12, 146);
            this.buttonRequestNDBs.Name = "buttonRequestNDBs";
            this.buttonRequestNDBs.Size = new System.Drawing.Size(131, 23);
            this.buttonRequestNDBs.TabIndex = 7;
            this.buttonRequestNDBs.Text = "Request NDBs";
            this.buttonRequestNDBs.UseVisualStyleBackColor = true;
            this.buttonRequestNDBs.Click += new System.EventHandler(this.buttonRequestNDBs_Click);
            // 
            // labelSubscriptions
            // 
            this.labelSubscriptions.AutoSize = true;
            this.labelSubscriptions.Location = new System.Drawing.Point(9, 241);
            this.labelSubscriptions.Name = "labelSubscriptions";
            this.labelSubscriptions.Size = new System.Drawing.Size(73, 13);
            this.labelSubscriptions.TabIndex = 8;
            this.labelSubscriptions.Text = "Subscriptions:";
            // 
            // checkboxAirportSubscription
            // 
            this.checkboxAirportSubscription.AutoSize = true;
            this.checkboxAirportSubscription.Location = new System.Drawing.Point(21, 270);
            this.checkboxAirportSubscription.Name = "checkboxAirportSubscription";
            this.checkboxAirportSubscription.Size = new System.Drawing.Size(61, 17);
            this.checkboxAirportSubscription.TabIndex = 9;
            this.checkboxAirportSubscription.Text = "Airports";
            this.checkboxAirportSubscription.UseVisualStyleBackColor = true;
            this.checkboxAirportSubscription.CheckedChanged += new System.EventHandler(this.checkboxAirportSubscription_CheckedChanged);
            // 
            // checkboxWaypointsSubscription
            // 
            this.checkboxWaypointsSubscription.AutoSize = true;
            this.checkboxWaypointsSubscription.Location = new System.Drawing.Point(21, 293);
            this.checkboxWaypointsSubscription.Name = "checkboxWaypointsSubscription";
            this.checkboxWaypointsSubscription.Size = new System.Drawing.Size(76, 17);
            this.checkboxWaypointsSubscription.TabIndex = 10;
            this.checkboxWaypointsSubscription.Text = "Waypoints";
            this.checkboxWaypointsSubscription.UseVisualStyleBackColor = true;
            this.checkboxWaypointsSubscription.CheckedChanged += new System.EventHandler(this.checkboxWaypointsSubscription_CheckedChanged);
            // 
            // checkboxNDBsSubscription
            // 
            this.checkboxNDBsSubscription.AutoSize = true;
            this.checkboxNDBsSubscription.Location = new System.Drawing.Point(21, 316);
            this.checkboxNDBsSubscription.Name = "checkboxNDBsSubscription";
            this.checkboxNDBsSubscription.Size = new System.Drawing.Size(54, 17);
            this.checkboxNDBsSubscription.TabIndex = 11;
            this.checkboxNDBsSubscription.Text = "NDBs";
            this.checkboxNDBsSubscription.UseVisualStyleBackColor = true;
            this.checkboxNDBsSubscription.CheckedChanged += new System.EventHandler(this.checkboxNDBsSubscription_CheckedChanged);
            // 
            // checkboxVORsSubscription
            // 
            this.checkboxVORsSubscription.AutoSize = true;
            this.checkboxVORsSubscription.Location = new System.Drawing.Point(21, 339);
            this.checkboxVORsSubscription.Name = "checkboxVORsSubscription";
            this.checkboxVORsSubscription.Size = new System.Drawing.Size(54, 17);
            this.checkboxVORsSubscription.TabIndex = 12;
            this.checkboxVORsSubscription.Text = "VORs";
            this.checkboxVORsSubscription.UseVisualStyleBackColor = true;
            this.checkboxVORsSubscription.CheckedChanged += new System.EventHandler(this.checkboxVORsSubscription_CheckedChanged);
            // 
            // buttonRequestTacans
            // 
            this.buttonRequestTacans.Location = new System.Drawing.Point(12, 204);
            this.buttonRequestTacans.Name = "buttonRequestTacans";
            this.buttonRequestTacans.Size = new System.Drawing.Size(131, 23);
            this.buttonRequestTacans.TabIndex = 13;
            this.buttonRequestTacans.Text = "Request Tacans";
            this.buttonRequestTacans.UseVisualStyleBackColor = true;
            this.buttonRequestTacans.Click += new System.EventHandler(this.buttonRequestTacans_Click);
            // 
            // checkboxTacansSubscription
            // 
            this.checkboxTacansSubscription.AutoSize = true;
            this.checkboxTacansSubscription.Location = new System.Drawing.Point(21, 362);
            this.checkboxTacansSubscription.Name = "checkboxTacansSubscription";
            this.checkboxTacansSubscription.Size = new System.Drawing.Size(62, 17);
            this.checkboxTacansSubscription.TabIndex = 14;
            this.checkboxTacansSubscription.Text = "Tacans";
            this.checkboxTacansSubscription.UseVisualStyleBackColor = true;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(907, 449);
            this.Controls.Add(this.checkboxTacansSubscription);
            this.Controls.Add(this.buttonRequestTacans);
            this.Controls.Add(this.checkboxVORsSubscription);
            this.Controls.Add(this.checkboxNDBsSubscription);
            this.Controls.Add(this.checkboxWaypointsSubscription);
            this.Controls.Add(this.checkboxAirportSubscription);
            this.Controls.Add(this.labelSubscriptions);
            this.Controls.Add(this.buttonRequestNDBs);
            this.Controls.Add(this.buttonRequestVORs);
            this.Controls.Add(this.buttonRequestWaypoints);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.richResponse);
            this.Controls.Add(this.buttonRequestAirports);
            this.Controls.Add(this.buttonDisconnect);
            this.Controls.Add(this.buttonConnect);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.SizableToolWindow;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MinimumSize = new System.Drawing.Size(300, 417);
            this.Name = "Form1";
            this.Text = "  SimConnect Managed Facilities Request";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Form1_FormClosed);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonConnect;
        private System.Windows.Forms.Button buttonDisconnect;
        private System.Windows.Forms.Button buttonRequestAirports;
        private System.Windows.Forms.RichTextBox richResponse;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button buttonRequestWaypoints;
        private System.Windows.Forms.Button buttonRequestVORs;
        private System.Windows.Forms.Button buttonRequestNDBs;
        private System.Windows.Forms.Label labelSubscriptions;
        private System.Windows.Forms.CheckBox checkboxAirportSubscription;
        private System.Windows.Forms.CheckBox checkboxWaypointsSubscription;
        private System.Windows.Forms.CheckBox checkboxNDBsSubscription;
        private System.Windows.Forms.CheckBox checkboxVORsSubscription;
        private System.Windows.Forms.Button buttonRequestTacans;
        private System.Windows.Forms.CheckBox checkboxTacansSubscription;
    }
}

