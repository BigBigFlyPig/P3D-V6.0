//Copyright (c) Lockheed Martin Corporation.  All rights reserved. 
namespace Managed_AI_Waypoints
{
    partial class AI_Waypoints
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(AI_Waypoints));
            this.buttonConnect = new System.Windows.Forms.Button();
            this.buttonDisconnect = new System.Windows.Forms.Button();
            this.buttonCreateAIObjects = new System.Windows.Forms.Button();
            this.richResponse = new System.Windows.Forms.RichTextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.buttonSendWaypoints = new System.Windows.Forms.Button();
            this.buttonLights = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // buttonConnect
            // 
            this.buttonConnect.Location = new System.Drawing.Point(12, 38);
            this.buttonConnect.Name = "buttonConnect";
            this.buttonConnect.Size = new System.Drawing.Size(131, 23);
            this.buttonConnect.TabIndex = 0;
            this.buttonConnect.Text = "Connect to P3D";
            this.buttonConnect.UseVisualStyleBackColor = true;
            this.buttonConnect.Click += new System.EventHandler(this.buttonConnect_Click);
            // 
            // buttonDisconnect
            // 
            this.buttonDisconnect.Location = new System.Drawing.Point(12, 241);
            this.buttonDisconnect.Name = "buttonDisconnect";
            this.buttonDisconnect.Size = new System.Drawing.Size(131, 23);
            this.buttonDisconnect.TabIndex = 1;
            this.buttonDisconnect.Text = "Disconnect from P3D";
            this.buttonDisconnect.UseVisualStyleBackColor = true;
            this.buttonDisconnect.Click += new System.EventHandler(this.buttonDisconnect_Click);
            // 
            // buttonCreateAIObjects
            // 
            this.buttonCreateAIObjects.Location = new System.Drawing.Point(12, 67);
            this.buttonCreateAIObjects.Name = "buttonCreateAIObjects";
            this.buttonCreateAIObjects.Size = new System.Drawing.Size(131, 72);
            this.buttonCreateAIObjects.TabIndex = 2;
            this.buttonCreateAIObjects.Text = "Create AI controlled Maule, stationary Mooney,  and a fuel truck";
            this.buttonCreateAIObjects.UseVisualStyleBackColor = true;
            this.buttonCreateAIObjects.Click += new System.EventHandler(this.buttonCreateAIObjects_Click);
            // 
            // richResponse
            // 
            this.richResponse.Location = new System.Drawing.Point(149, 40);
            this.richResponse.Name = "richResponse";
            this.richResponse.ReadOnly = true;
            this.richResponse.Size = new System.Drawing.Size(221, 224);
            this.richResponse.TabIndex = 3;
            this.richResponse.Text = "";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(149, 21);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(60, 13);
            this.label1.TabIndex = 4;
            this.label1.Text = "Responses";
            // 
            // buttonSendWaypoints
            // 
            this.buttonSendWaypoints.Location = new System.Drawing.Point(12, 188);
            this.buttonSendWaypoints.Name = "buttonSendWaypoints";
            this.buttonSendWaypoints.Size = new System.Drawing.Size(131, 47);
            this.buttonSendWaypoints.TabIndex = 5;
            this.buttonSendWaypoints.Text = "Send waypoint list to Maule and fuel truck";
            this.buttonSendWaypoints.UseVisualStyleBackColor = true;
            this.buttonSendWaypoints.Click += new System.EventHandler(this.buttonSendWaypoints_Click);
            // 
            // buttonLights
            // 
            this.buttonLights.Location = new System.Drawing.Point(12, 145);
            this.buttonLights.Name = "buttonLights";
            this.buttonLights.Size = new System.Drawing.Size(131, 37);
            this.buttonLights.TabIndex = 6;
            this.buttonLights.Text = "Send toggle lights request to Maule";
            this.buttonLights.UseVisualStyleBackColor = true;
            this.buttonLights.Click += new System.EventHandler(this.buttonLights_Click);
            // 
            // AI_Waypoints
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.Khaki;
            this.ClientSize = new System.Drawing.Size(393, 296);
            this.Controls.Add(this.buttonLights);
            this.Controls.Add(this.buttonSendWaypoints);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.richResponse);
            this.Controls.Add(this.buttonCreateAIObjects);
            this.Controls.Add(this.buttonDisconnect);
            this.Controls.Add(this.buttonConnect);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "AI_Waypoints";
            this.Text = "  SimConnect Managed AI Waypoints";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Form1_FormClosed);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonConnect;
        private System.Windows.Forms.Button buttonDisconnect;
        private System.Windows.Forms.Button buttonCreateAIObjects;
        private System.Windows.Forms.RichTextBox richResponse;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button buttonSendWaypoints;
        private System.Windows.Forms.Button buttonLights;
    }
}

