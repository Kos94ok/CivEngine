namespace Taiga_Editor
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
            System.Windows.Forms.TreeNode treeNode4 = new System.Windows.Forms.TreeNode("Units");
            System.Windows.Forms.TreeNode treeNode5 = new System.Windows.Forms.TreeNode("Items");
            System.Windows.Forms.TreeNode treeNode6 = new System.Windows.Forms.TreeNode("UI");
            this.treeView = new System.Windows.Forms.TreeView();
            this.btn_addElement = new System.Windows.Forms.Button();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tab_unit = new System.Windows.Forms.TabPage();
            this.text_unit_name = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.tab_item = new System.Windows.Forms.TabPage();
            this.tab_ui = new System.Windows.Forms.TabPage();
            this.btn_removeElement = new System.Windows.Forms.Button();
            this.tabControl1.SuspendLayout();
            this.tab_unit.SuspendLayout();
            this.SuspendLayout();
            // 
            // treeView
            // 
            this.treeView.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.treeView.Location = new System.Drawing.Point(12, 12);
            this.treeView.MaximumSize = new System.Drawing.Size(300, 400);
            this.treeView.MinimumSize = new System.Drawing.Size(100, 400);
            this.treeView.Name = "treeView";
            treeNode4.Name = "node_units";
            treeNode4.Text = "Units";
            treeNode5.Name = "node_items";
            treeNode5.Text = "Items";
            treeNode6.Name = "node_ui";
            treeNode6.Text = "UI";
            this.treeView.Nodes.AddRange(new System.Windows.Forms.TreeNode[] {
            treeNode4,
            treeNode5,
            treeNode6});
            this.treeView.Size = new System.Drawing.Size(143, 400);
            this.treeView.TabIndex = 0;
            this.treeView.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.treeView1_AfterSelect);
            // 
            // btn_addElement
            // 
            this.btn_addElement.Location = new System.Drawing.Point(12, 415);
            this.btn_addElement.Name = "btn_addElement";
            this.btn_addElement.Size = new System.Drawing.Size(60, 23);
            this.btn_addElement.TabIndex = 1;
            this.btn_addElement.Text = "Add";
            this.btn_addElement.UseVisualStyleBackColor = true;
            this.btn_addElement.Click += new System.EventHandler(this.btn_addElement_Click);
            // 
            // tabControl1
            // 
            this.tabControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tabControl1.Controls.Add(this.tab_unit);
            this.tabControl1.Controls.Add(this.tab_item);
            this.tabControl1.Controls.Add(this.tab_ui);
            this.tabControl1.Location = new System.Drawing.Point(161, 12);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(442, 426);
            this.tabControl1.TabIndex = 2;
            // 
            // tab_unit
            // 
            this.tab_unit.Controls.Add(this.text_unit_name);
            this.tab_unit.Controls.Add(this.label1);
            this.tab_unit.Location = new System.Drawing.Point(4, 22);
            this.tab_unit.Name = "tab_unit";
            this.tab_unit.Padding = new System.Windows.Forms.Padding(3);
            this.tab_unit.Size = new System.Drawing.Size(434, 400);
            this.tab_unit.TabIndex = 0;
            this.tab_unit.Text = "Unit";
            this.tab_unit.UseVisualStyleBackColor = true;
            // 
            // text_unit_name
            // 
            this.text_unit_name.Location = new System.Drawing.Point(114, 7);
            this.text_unit_name.Name = "text_unit_name";
            this.text_unit_name.Size = new System.Drawing.Size(100, 20);
            this.text_unit_name.TabIndex = 1;
            this.text_unit_name.TextChanged += new System.EventHandler(this.text_unit_name_TextChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(8, 10);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(38, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Name:";
            // 
            // tab_item
            // 
            this.tab_item.Location = new System.Drawing.Point(4, 22);
            this.tab_item.Name = "tab_item";
            this.tab_item.Padding = new System.Windows.Forms.Padding(3);
            this.tab_item.Size = new System.Drawing.Size(434, 400);
            this.tab_item.TabIndex = 1;
            this.tab_item.Text = "Item";
            this.tab_item.UseVisualStyleBackColor = true;
            // 
            // tab_ui
            // 
            this.tab_ui.Location = new System.Drawing.Point(4, 22);
            this.tab_ui.Name = "tab_ui";
            this.tab_ui.Size = new System.Drawing.Size(434, 400);
            this.tab_ui.TabIndex = 2;
            this.tab_ui.Text = "UI Element";
            this.tab_ui.UseVisualStyleBackColor = true;
            // 
            // btn_removeElement
            // 
            this.btn_removeElement.Location = new System.Drawing.Point(95, 415);
            this.btn_removeElement.Name = "btn_removeElement";
            this.btn_removeElement.Size = new System.Drawing.Size(60, 23);
            this.btn_removeElement.TabIndex = 3;
            this.btn_removeElement.Text = "Remove";
            this.btn_removeElement.UseVisualStyleBackColor = true;
            this.btn_removeElement.Click += new System.EventHandler(this.btn_removeElement_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoScroll = true;
            this.ClientSize = new System.Drawing.Size(615, 450);
            this.Controls.Add(this.btn_removeElement);
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.btn_addElement);
            this.Controls.Add(this.treeView);
            this.Name = "Form1";
            this.Text = "Form1";
            this.tabControl1.ResumeLayout(false);
            this.tab_unit.ResumeLayout(false);
            this.tab_unit.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        public System.Windows.Forms.TreeView treeView;
        private System.Windows.Forms.Button btn_addElement;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tab_unit;
        private System.Windows.Forms.TabPage tab_item;
        private System.Windows.Forms.TabPage tab_ui;
        private System.Windows.Forms.TextBox text_unit_name;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btn_removeElement;
    }
}

