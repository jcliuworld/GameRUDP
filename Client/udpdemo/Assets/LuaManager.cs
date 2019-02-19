using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;
using UnityEngine.UI;
using XLua;

public class LuaManager : MonoBehaviour {

    // Use this for initialization
    private LuaEnv luaenv;
    private Action sendFunc;
    private Action closeFunc;
    private Action syncFunc;

    private LuaTable scriptEnv;
    public Button send;

    void Start () {
        GameObject.DontDestroyOnLoad(this);
        luaenv = new LuaEnv();

        scriptEnv = luaenv.NewTable();
        LuaTable meta = luaenv.NewTable();
        meta.Set("__index", luaenv.Global);
        scriptEnv.SetMetaTable(meta);
        meta.Dispose();

        scriptEnv.Set("self", this);

        luaenv.AddBuildin("pb", XLua.LuaDLL.Lua.LoadLuaProfobuf);
        luaenv.AddBuildin("socketevent", XLua.LuaDLL.Lua.LoadRUDPSocket);

        //luaenv.AddBuildin("socket", XLua.LuaDLL.Lua.LoadLuaSocket);

        luaenv.AddLoader((ref string filename) =>
        {
            string path = string.Format("{0}/Lua/{1}.lua", Application.dataPath, filename);
            byte[] bytes = File.ReadAllBytes(path);

            return bytes;
        });


        luaenv.DoString(@"require('main')", "LuaBehaviour", scriptEnv);
        Action main = luaenv.Global.Get<Action>("main");
      
        main();
        
        if(send != null)
        {
            send.onClick.AddListener(SendMsg);
        }

      
        scriptEnv.Get("sendmsg", out sendFunc);

        scriptEnv.Get("close", out closeFunc);


        scriptEnv.Get("sync", out syncFunc);
    }

    // Update is called once per frame
    void SendMsg () {

        Debug.Log("send");

        if (sendFunc != null)
        {
            sendFunc();
        }


    }

    public void Update()
    {
        if(syncFunc != null)
        {
            syncFunc();
        }
    }

    private void OnDestroy()
    {

        if(closeFunc != null)
        {
            closeFunc();
        }
      
    }
}
