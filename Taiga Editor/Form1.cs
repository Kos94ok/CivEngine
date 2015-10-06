using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Taiga_Editor
{
    public partial class Form1 : Form
    {
        int selectionType = -1;
        int selectionIndex = -1;
        // Initialization
        public cDatabase database = new cDatabase();
        public Form1()
        {
            InitializeComponent();

            database.parent = this;
            database.addUnit("missingno");
            database.addItem("missingno");
            database.addUIElement("missingno");
        }

        // Tree view object selection
        private void treeView1_AfterSelect(object sender, TreeViewEventArgs e)
        {
            tabControl1.TabPages.Remove(tab_unit);
            tabControl1.TabPages.Remove(tab_item);
            tabControl1.TabPages.Remove(tab_ui);
            if (treeView.SelectedNode.Parent != null)
            {
                tabControl1.Show();
                if (treeView.SelectedNode.Parent.Name == "node_units")
                {
                    selectionType = 0;
                    selectionIndex = treeView.SelectedNode.Index;
                    tabControl1.TabPages.Add(tab_unit);
                    text_unit_name.Text = "missingno";
                }
                else if (treeView.SelectedNode.Parent.Name == "node_items")
                {
                    selectionType = 1;
                    selectionIndex = treeView.SelectedNode.Index;
                    tabControl1.TabPages.Add(tab_item);
                }
                else if (treeView.SelectedNode.Parent.Name == "node_ui")
                {
                    selectionType = 2;
                    selectionIndex = treeView.SelectedNode.Index;
                    tabControl1.TabPages.Add(tab_ui);
                }
            }
            else
            {
                tabControl1.Hide();
            }
        }

        // Add new element button
        private void btn_addElement_Click(object sender, EventArgs e)
        {
            treeView.BeginUpdate();
            // New unit
            if (treeView.SelectedNode.Name == "node_units")
            {
                database.addUnit("missingno");
            }
            // New item
            else if (treeView.SelectedNode.Name == "node_items")
            {
                database.addItem("missingno");
            }
            // New UI element
            else if (treeView.SelectedNode.Name == "node_ui")
            {
                database.addUIElement("missingno");
            }
            treeView.EndUpdate();
        }

        // Remove element button
        private void btn_removeElement_Click(object sender, EventArgs e)
        {
            if (treeView.SelectedNode != null && treeView.SelectedNode.Parent != null)
            {
                // Units
                if (treeView.SelectedNode.Parent.Name == "node_units")
                {
                    database.removeUnitSel();
                }
                // Items
                else if (treeView.SelectedNode.Parent.Name == "node_items")
                {
                    database.removeItemSel();
                }
                // UI Elements
                else if (treeView.SelectedNode.Parent.Name == "node_ui")
                {
                    database.removeUIElementSel();
                }
            }
        }

        // Change unit name
        private void text_unit_name_TextChanged(object sender, EventArgs e)
        {
            if (selectionType == 0) 
            {
                database.setUnitName(selectionIndex, text_unit_name.Text);
            }
        }
    }
}
