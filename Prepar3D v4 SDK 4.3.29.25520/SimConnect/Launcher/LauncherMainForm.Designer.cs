//Copyright (c) Lockheed Martin Corporation.  All rights reserved.  

namespace Launcher
{
    partial class Launcher
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
           System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Launcher));
           this.ScenarioList = new System.Windows.Forms.ListBox();
           this.Join = new System.Windows.Forms.Button();
           this.FlySolo = new System.Windows.Forms.Button();
           this.HostMP = new System.Windows.Forms.Button();
           this.groupBox1 = new System.Windows.Forms.GroupBox();
           this.JoinOn = new System.Windows.Forms.Button();
           this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
           this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
           this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
           this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
           this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
           this.solutionInfoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
           this.menuStrip1 = new System.Windows.Forms.MenuStrip();
           this.MainPanel = new System.Windows.Forms.Panel();
           this.panel1 = new System.Windows.Forms.Panel();
           this.ScenarioDescription = new System.Windows.Forms.TextBox();
           this.ScenarioPicture = new System.Windows.Forms.PictureBox();
           this.Next = new System.Windows.Forms.Button();
           this.NextOn = new System.Windows.Forms.Button();
           this.HostMPOn = new System.Windows.Forms.Button();
           this.FlySoloOn = new System.Windows.Forms.Button();
           this.DevModeLabel = new System.Windows.Forms.Label();
           this.groupBox1.SuspendLayout();
           this.menuStrip1.SuspendLayout();
           this.MainPanel.SuspendLayout();
           this.panel1.SuspendLayout();
           ((System.ComponentModel.ISupportInitialize)(this.ScenarioPicture)).BeginInit();
           this.SuspendLayout();
           // 
           // ScenarioList
           // 
           resources.ApplyResources(this.ScenarioList, "ScenarioList");
           this.ScenarioList.FormattingEnabled = true;
           this.ScenarioList.Name = "ScenarioList";
           this.ScenarioList.SelectedIndexChanged += new System.EventHandler(this.ScenarioList_SelectedIndexChanged);
           // 
           // Join
           // 
           this.Join.FlatAppearance.BorderColor = System.Drawing.Color.RoyalBlue;
           this.Join.FlatAppearance.BorderSize = 0;
           this.Join.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
           this.Join.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
           resources.ApplyResources(this.Join, "Join");
           this.Join.ForeColor = System.Drawing.Color.Transparent;
           this.Join.Image = global::Launcher.Properties.Resources.btnjoinmp_off;
           this.Join.Name = "Join";
           this.Join.UseVisualStyleBackColor = true;
           this.Join.Click += new System.EventHandler(this.Join_Click);
           // 
           // FlySolo
           // 
           resources.ApplyResources(this.FlySolo, "FlySolo");
           this.FlySolo.FlatAppearance.BorderColor = System.Drawing.Color.RoyalBlue;
           this.FlySolo.FlatAppearance.BorderSize = 0;
           this.FlySolo.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
           this.FlySolo.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
           this.FlySolo.ForeColor = System.Drawing.Color.Transparent;
           this.FlySolo.Image = global::Launcher.Properties.Resources.btnflysolo_off;
           this.FlySolo.Name = "FlySolo";
           this.FlySolo.UseVisualStyleBackColor = true;
           this.FlySolo.Click += new System.EventHandler(this.FlySolo_Click);
           // 
           // HostMP
           // 
           this.HostMP.FlatAppearance.BorderColor = System.Drawing.Color.RoyalBlue;
           this.HostMP.FlatAppearance.BorderSize = 0;
           this.HostMP.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
           this.HostMP.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
           resources.ApplyResources(this.HostMP, "HostMP");
           this.HostMP.ForeColor = System.Drawing.Color.Transparent;
           this.HostMP.Image = global::Launcher.Properties.Resources.btnhostmp_off;
           this.HostMP.Name = "HostMP";
           this.HostMP.UseVisualStyleBackColor = true;
           this.HostMP.Click += new System.EventHandler(this.HostMP_Click);
           // 
           // groupBox1
           // 
           this.groupBox1.Controls.Add(this.ScenarioList);
           resources.ApplyResources(this.groupBox1, "groupBox1");
           this.groupBox1.Name = "groupBox1";
           this.groupBox1.TabStop = false;
           // 
           // JoinOn
           // 
           this.JoinOn.FlatAppearance.BorderColor = System.Drawing.Color.RoyalBlue;
           this.JoinOn.FlatAppearance.BorderSize = 0;
           this.JoinOn.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
           this.JoinOn.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
           resources.ApplyResources(this.JoinOn, "JoinOn");
           this.JoinOn.ForeColor = System.Drawing.Color.Transparent;
           this.JoinOn.Image = global::Launcher.Properties.Resources.btnjoinmp_on;
           this.JoinOn.Name = "JoinOn";
           this.JoinOn.UseVisualStyleBackColor = true;
           // 
           // fileToolStripMenuItem
           // 
           this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openToolStripMenuItem,
            this.exitToolStripMenuItem});
           this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
           resources.ApplyResources(this.fileToolStripMenuItem, "fileToolStripMenuItem");
           // 
           // openToolStripMenuItem
           // 
           this.openToolStripMenuItem.Name = "openToolStripMenuItem";
           resources.ApplyResources(this.openToolStripMenuItem, "openToolStripMenuItem");
           this.openToolStripMenuItem.Click += new System.EventHandler(this.openToolStripMenuItem_Click);
           // 
           // exitToolStripMenuItem
           // 
           this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
           resources.ApplyResources(this.exitToolStripMenuItem, "exitToolStripMenuItem");
           this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
           // 
           // helpToolStripMenuItem
           // 
           this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.aboutToolStripMenuItem,
            this.solutionInfoToolStripMenuItem});
           this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
           resources.ApplyResources(this.helpToolStripMenuItem, "helpToolStripMenuItem");
           // 
           // aboutToolStripMenuItem
           // 
           this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
           resources.ApplyResources(this.aboutToolStripMenuItem, "aboutToolStripMenuItem");
           this.aboutToolStripMenuItem.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
           // 
           // solutionInfoToolStripMenuItem
           // 
           this.solutionInfoToolStripMenuItem.Name = "solutionInfoToolStripMenuItem";
           resources.ApplyResources(this.solutionInfoToolStripMenuItem, "solutionInfoToolStripMenuItem");
           this.solutionInfoToolStripMenuItem.Click += new System.EventHandler(this.solutionInfoToolStripMenuItem_Click);
           // 
           // menuStrip1
           // 
           this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.helpToolStripMenuItem});
           resources.ApplyResources(this.menuStrip1, "menuStrip1");
           this.menuStrip1.Name = "menuStrip1";
           // 
           // MainPanel
           // 
           resources.ApplyResources(this.MainPanel, "MainPanel");
           this.MainPanel.BackColor = System.Drawing.Color.Transparent;
           this.MainPanel.Controls.Add(this.groupBox1);
           this.MainPanel.Controls.Add(this.panel1);
           this.MainPanel.Controls.Add(this.Join);
           this.MainPanel.Controls.Add(this.JoinOn);
           this.MainPanel.Controls.Add(this.HostMP);
           this.MainPanel.Controls.Add(this.HostMPOn);
           this.MainPanel.Controls.Add(this.FlySolo);
           this.MainPanel.Controls.Add(this.FlySoloOn);
           this.MainPanel.MinimumSize = new System.Drawing.Size(954, 524);
           this.MainPanel.Name = "MainPanel";
           // 
           // panel1
           // 
           this.panel1.BackColor = System.Drawing.Color.Transparent;
           this.panel1.Controls.Add(this.ScenarioDescription);
           this.panel1.Controls.Add(this.ScenarioPicture);
           this.panel1.Controls.Add(this.Next);
           this.panel1.Controls.Add(this.NextOn);
           this.panel1.ForeColor = System.Drawing.Color.Transparent;
           resources.ApplyResources(this.panel1, "panel1");
           this.panel1.Name = "panel1";
           // 
           // ScenarioDescription
           // 
           this.ScenarioDescription.BackColor = System.Drawing.Color.White;
           this.ScenarioDescription.BorderStyle = System.Windows.Forms.BorderStyle.None;
           resources.ApplyResources(this.ScenarioDescription, "ScenarioDescription");
           this.ScenarioDescription.ForeColor = System.Drawing.Color.Black;
           this.ScenarioDescription.Name = "ScenarioDescription";
           // 
           // ScenarioPicture
           // 
           resources.ApplyResources(this.ScenarioPicture, "ScenarioPicture");
           this.ScenarioPicture.BackgroundImage = global::Launcher.Properties.Resources._default;
           this.ScenarioPicture.Name = "ScenarioPicture";
           this.ScenarioPicture.TabStop = false;
           // 
           // Next
           // 
           this.Next.FlatAppearance.BorderSize = 0;
           this.Next.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
           this.Next.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
           resources.ApplyResources(this.Next, "Next");
           this.Next.ForeColor = System.Drawing.Color.Transparent;
           this.Next.Image = global::Launcher.Properties.Resources.BTNLAUNCH_OFF;
           this.Next.Name = "Next";
           this.Next.UseVisualStyleBackColor = true;
           this.Next.Click += new System.EventHandler(this.Next_Click);
           // 
           // NextOn
           // 
           resources.ApplyResources(this.NextOn, "NextOn");
           this.NextOn.FlatAppearance.BorderColor = System.Drawing.SystemColors.ActiveCaption;
           this.NextOn.FlatAppearance.BorderSize = 0;
           this.NextOn.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
           this.NextOn.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
           this.NextOn.ForeColor = System.Drawing.Color.Transparent;
           this.NextOn.Image = global::Launcher.Properties.Resources.BTNLAUNCH_ON;
           this.NextOn.Name = "NextOn";
           this.NextOn.UseVisualStyleBackColor = true;
           // 
           // HostMPOn
           // 
           this.HostMPOn.FlatAppearance.BorderColor = System.Drawing.Color.RoyalBlue;
           this.HostMPOn.FlatAppearance.BorderSize = 0;
           this.HostMPOn.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
           this.HostMPOn.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
           resources.ApplyResources(this.HostMPOn, "HostMPOn");
           this.HostMPOn.ForeColor = System.Drawing.Color.Transparent;
           this.HostMPOn.Image = global::Launcher.Properties.Resources.btnhostmp_on;
           this.HostMPOn.Name = "HostMPOn";
           this.HostMPOn.UseVisualStyleBackColor = true;
           // 
           // FlySoloOn
           // 
           resources.ApplyResources(this.FlySoloOn, "FlySoloOn");
           this.FlySoloOn.FlatAppearance.BorderColor = System.Drawing.Color.RoyalBlue;
           this.FlySoloOn.FlatAppearance.BorderSize = 0;
           this.FlySoloOn.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
           this.FlySoloOn.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
           this.FlySoloOn.ForeColor = System.Drawing.Color.Transparent;
           this.FlySoloOn.Image = global::Launcher.Properties.Resources.btnflysolo_on;
           this.FlySoloOn.Name = "FlySoloOn";
           this.FlySoloOn.UseVisualStyleBackColor = true;
           // 
           // DevModeLabel
           // 
           resources.ApplyResources(this.DevModeLabel, "DevModeLabel");
           this.DevModeLabel.BackColor = System.Drawing.Color.Transparent;
           this.DevModeLabel.Name = "DevModeLabel";
           // 
           // Launcher
           // 
           resources.ApplyResources(this, "$this");
           this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
           this.BackColor = System.Drawing.SystemColors.ActiveCaption;
           this.Controls.Add(this.DevModeLabel);
           this.Controls.Add(this.MainPanel);
           this.Controls.Add(this.menuStrip1);
           this.MainMenuStrip = this.menuStrip1;
           this.Name = "Launcher";
           this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
           this.Load += new System.EventHandler(this.Launcher_Load);
           this.groupBox1.ResumeLayout(false);
           this.menuStrip1.ResumeLayout(false);
           this.menuStrip1.PerformLayout();
           this.MainPanel.ResumeLayout(false);
           this.panel1.ResumeLayout(false);
           this.panel1.PerformLayout();
           ((System.ComponentModel.ISupportInitialize)(this.ScenarioPicture)).EndInit();
           this.ResumeLayout(false);
           this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ListBox ScenarioList;
        private System.Windows.Forms.Button Join;
        private System.Windows.Forms.Button FlySolo;
        private System.Windows.Forms.Button HostMP;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem solutionInfoToolStripMenuItem;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.Panel MainPanel;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button Next;
        private System.Windows.Forms.TextBox ScenarioDescription;
        private System.Windows.Forms.PictureBox ScenarioPicture;
        private System.Windows.Forms.Button FlySoloOn;
        private System.Windows.Forms.Button HostMPOn;
        private System.Windows.Forms.Button JoinOn;
        private System.Windows.Forms.Button NextOn;
        private System.Windows.Forms.Label DevModeLabel;
    }
}

