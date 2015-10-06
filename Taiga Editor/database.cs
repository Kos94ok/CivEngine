using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Taiga_Editor
{
    // Header
    public class cDBObject
    {
        public bool isValid = false;
        public string name = new string('0', 1);
    }
    public class cUnit : cDBObject
    {
        string name { get; set; }
    }
    public class cItem : cDBObject
    {
    }

    public class cUIElement : cDBObject
    {

    }

    public class cDatabase
    {
        public Form1 parent;

        public int unitCount = 0;
        public cUnit[] unit = new cUnit[256];
        public int itemCount = 0;
        public cItem[] item = new cItem[256];
        public int uiElementCount = 0;
        public cUIElement[] uiElement = new cUIElement[256];

        // Add object
        public int addUnit(string name)
        {
            int id = 0;
            for (int i = 0; i < 256; i++)
            {
                if (!unit[i].isValid)
                {
                    id = i;
                    unit[id].name = name;
                    unit[id].isValid = true;
                    parent.treeView.Nodes.Find("node_units", true)[0].Nodes.Add(id.ToString(), name);
                    i = 256;
                }
            }
            return id;
        }
        public int addItem(string name)
        {
            int id = 0;
            for (int i = 0; i < 256; i++)
            {
                if (!item[i].isValid)
                {
                    id = i;
                    item[id].name = name;
                    item[id].isValid = true;
                    parent.treeView.Nodes.Find("node_items", true)[0].Nodes.Add(id.ToString(), name);
                    i = 256;
                }
            }
            return id;
        }
        public int addUIElement(string name)
        {
            int id = 0;
            for (int i = 0; i < 256; i++)
            {
                if (!uiElement[i].isValid)
                {
                    id = i;
                    uiElement[id].name = name;
                    uiElement[id].isValid = true;
                    parent.treeView.Nodes.Find("node_ui", true)[0].Nodes.Add(id.ToString(), name);
                    i = 256;
                }
            }
            return id;
        }

        // Remove object
        public bool removeUnitSel()
        {
            if (parent.treeView.SelectedNode == null
                || parent.treeView.SelectedNode.Parent == null) { return false; }

            unit[Convert.ToInt32(parent.treeView.SelectedNode.Name)].isValid = false;
            parent.treeView.Nodes.Remove(parent.treeView.SelectedNode);

            return true;
        }
        public bool removeItemSel()
        {
            if (parent.treeView.SelectedNode == null
                || parent.treeView.SelectedNode.Parent == null) { return false; }

            item[Convert.ToInt32(parent.treeView.SelectedNode.Name)].isValid = false;
            parent.treeView.Nodes.Remove(parent.treeView.SelectedNode);

            return true;
        }
        public bool removeUIElementSel()
        {
            if (parent.treeView.SelectedNode == null
                || parent.treeView.SelectedNode.Parent == null) { return false; }

            uiElement[Convert.ToInt32(parent.treeView.SelectedNode.Name)].isValid = false;
            parent.treeView.Nodes.Remove(parent.treeView.SelectedNode);

            return true;
        }

        // Set object name
        public void setUnitName(int id, string value)
        {
            unit[id].name = value;
            parent.treeView.SelectedNode.Name = value;
        }

        // Constructor
        public cDatabase()
        {
            for (int i = 0; i < 256; i++)
            {
                unit[i] = new cUnit();
                item[i] = new cItem();
                uiElement[i] = new cUIElement();
            }
        }
    }
}
