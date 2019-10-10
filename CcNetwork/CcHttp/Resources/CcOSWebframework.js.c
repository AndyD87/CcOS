// Resource file generated from CcOSResource
#include "CcBase.h"
const char* CcOSWebframework_Js = "\
// Initialize gloabl page variables\n\
var Page_Application = Page_LoadApplicationDefault;\n\
var Page_RefreshPaused = false;\n\
var Page_RefreshTimer  = null;\n\
var Page_RefreshMethod = null;\n\
\n\
Page_ContentLoaded(window, Page_Init);\n\
\n\
function Page_ToggleRefreshLoop()\n\
{\n\
    if(Page_RefreshMethod!=null)\n\
    {\n\
        Page_RefreshPaused = !Page_RefreshPaused;\n\
    }\n\
}\n\
\n\
function Page_Init()\n\
{\n\
    if(Page_LoadFiles)\n\
    {\n\
        var oPageLoader = new CPageLoader(Page_LoadFiles);\n\
        oPageLoader.loadFiles();\n\
    }\n\
    Page_Application();\n\
\n\
    var oFooter = Util_GetDivByIdOrCreate(\"footer\");\n\
    var oEntry = document.createElement(\"span\");\n\
    oEntry.innerHTML = \"<b>||</b\";\n\
    oEntry.setAttribute('onclick', \"Page_ToggleRefreshLoop()\");\n\
    oFooter.appendChild(oEntry);\n\
}\n\
\n\
function Page_ContentLoaded(win, fn)\n\
{\n\
    var done = false, top = true,\n\
    doc = win.document,\n\
    root = doc.documentElement,\n\
    add = doc.addEventListener ? 'addEventListener' : 'attachEvent',\n\
    rem = doc.addEventListener ? 'removeEventListener' : 'detachEvent',\n\
    pre = doc.addEventListener ? '' : 'on',\n\
    init = function(e)\n\
    {\n\
        if (e.type == 'readystatechange' && doc.readyState != 'complete') return;\n\
        (e.type == 'load' ? win : doc)[rem](pre + e.type, init, false);\n\
        if (!done && (done = true)) fn.call(win, e.type || e);\n\
    },\n\
    poll = function()\n\
    {\n\
        try { root.doScroll('left'); } catch(e) { setTimeout(poll, 50); return; }\n\
        init('poll');\n\
    };\n\
    if (doc.readyState == 'complete') fn.call(win, 'lazy');\n\
    else {\n\
        if (doc.createEventObject && root.doScroll) {\n\
            try { top = !win.frameElement; } catch(e) { }\n\
            if (top) poll();\n\
        }\n\
        doc[add](pre + 'DOMPage_ContentLoaded', init, false);\n\
        doc[add](pre + 'readystatechange', init, false);\n\
        win[add](pre + 'load', init, false);\n\
    }\n\
}\n\
\n\
function Page_LoadApplication(sRestApiLink)\n\
{\n\
    if( sRestApiLink === null ||\n\
            !Page_Application(sRestApiLink))\n\
    {\n\
        Page_LoadApplicationDefault(sRestApiLink);\n\
        Page_StartRefreshLoop( function() { Page_LoadApplicationDefault(sRestApiLink); });\n\
    }\n\
}\n\
\n\
function Page_LoadApplicationDefault(sRestApiLink)\n\
{\n\
    if( sRestApiLink == null) sRestApiLink = '/api/app/status';\n\
    if( sRestApiLink.startsWith('/api/app/status'))\n\
    {\n\
        var oAjax = new CAjax();\n\
        var oResult = oAjax.get(sRestApiLink);\n\
        oAjax.onSuccess = function(oResult)\n\
        {\n\
            var oData = JSON.parse(oResult);\n\
            var oDevicesDiv = document.getElementById(\"content\");\n\
            if(oDevicesDiv !== null)\n\
            {\n\
                oDevicesDiv.innerText = oResult;\n\
            }\n\
        };\n\
        oAjax.get(sRestApiLink);\n\
    }\n\
}\n\
\n\
function CAjax()\n\
{\n\
    var CAjax_this = this;\n\
    this.get = function(sUrl)\n\
    {\n\
        if(CAjax.CurrentConnections < CAjax.MaxConnections)\n\
        {\n\
            CAjax.CurrentConnections++;\n\
            var xhr = new XMLHttpRequest();\n\
            xhr.open('GET', sUrl);\n\
            xhr.onload = function()\n\
            {\n\
                CAjax.CurrentConnections--;\n\
                if (xhr.status === 200)\n\
                {\n\
                    CAjax_this.onSuccess(xhr.responseText);\n\
                }\n\
                else\n\
                {\n\
                    CAjax_this.onError(xhr.status);\n\
                }\n\
                CAjax_this.sendNext();\n\
            };\n\
            xhr.send();\n\
        }\n\
        else\n\
        {\n\
            this.sUrl = sUrl;\n\
            this.appendRequest(CAjax_this);\n\
        }\n\
    };\n\
    this.post = function(sUrl, aDataArray)\n\
    {\n\
        var bFirst = 1;\n\
        var sParam = '';\n\
        for (var sKey in aDataArray)\n\
        {\n\
            if(bFirst == 1) bFirst = 0;\n\
            else sParam += '&';\n\
            sParam += sKey + '=' + aDataArray[sKey];\n\
        }\n\
        CAjax.CurrentConnections++;\n\
        var xhr = new XMLHttpRequest();\n\
        xhr.open('POST', sUrl, true);\n\
        xhr.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');\n\
        xhr.onload = function()\n\
        {\n\
            CAjax.CurrentConnections--;\n\
            if (xhr.status === 200)\n\
            {\n\
                CAjax_this.onSuccess(xhr.responseText);\n\
            }\n\
            else\n\
            {\n\
                CAjax_this.onError(xhr.status);\n\
            }\n\
        };\n\
        xhr.send(sParam);\n\
    };\n\
    this.onSuccess = function(sData)\n\
    {\n\
    };\n\
    this.onError = function(sData)\n\
    {\n\
        console.log('CAjax::onError:' + sData)\n\
    };\n\
\n\
    this.appendRequest = function(oAjax)\n\
    {\n\
        var bFound = 0;\n\
        for(var i=0; i<CAjax.Requests.length; i++)\n\
        {\n\
            if(CAjax.Requests[i].sUrl === oAjax.sUrl)\n\
            {\n\
                bFound = true;\n\
                break;\n\
            }\n\
        }\n\
        if(bFound === 0)\n\
        {\n\
            CAjax.Requests.push(oAjax);\n\
        }\n\
    };\n\
\n\
    this.sendNext = function()\n\
    {\n\
        if(CAjax.Requests.length)\n\
        {\n\
            var oAjax = CAjax.Requests.shift();\n\
            oAjax.get(oAjax.sUrl);\n\
        }\n\
    };\n\
}\n\
CAjax.Requests              = [];\n\
CAjax.MaxConnections        = 3;\n\
CAjax.CurrentConnections    = 0;\n\
\n\
var CPageLoader = (function ()\n\
{\n\
    function CPageLoader(files)\n\
    {\n\
        var _this = this;\n\
        this.loadStyle = function (filename)\n\
        {\n\
            // HTMLLinkElement\n\
            var link = document.createElement(\"link\");\n\
            link.rel = \"stylesheet\";\n\
            link.type = \"text/css\";\n\
            link.href = _filename;\n\
            link.onload = function ()\n\
            {\n\
            };\n\
            link.onerror = function ()\n\
            {\n\
            };\n\
            _this.m_head.appendChild(link);\n\
        };\n\
        this.loadScript = function (filename)\n\
        {\n\
            var script = document.createElement('script');\n\
            script.type = 'text/javascript';\n\
            script.src = filename;\n\
            script.onload = function ()\n\
            {\n\
            };\n\
            script.onerror = function ()\n\
            {\n\
            };\n\
            _this.m_head.appendChild(script);\n\
        };\n\
        this.loadFiles = function ()\n\
        {\n\
            for (var i = 0; i < _this.m_css_files.length; ++i)\n\
                _this.loadStyle(_this.m_css_files[i]);\n\
            for (var i = 0; i < _this.m_js_files.length; ++i)\n\
                _this.loadScript(_this.m_js_files[i]);\n\
        };\n\
        this.m_js_files = [];\n\
        this.m_css_files = [];\n\
        this.m_head = document.getElementsByTagName(\"head\")[0];\n\
        function endsWith(str, suffix)\n\
        {\n\
            if (str === null || suffix === null)\n\
                return false;\n\
            return str.indexOf(suffix, str.length - suffix.length) !== -1;\n\
        }\n\
        for (var i = 0; i < files.length; ++i)\n\
        {\n\
            if (endsWith(files[i], \".css\"))\n\
            {\n\
                this.m_css_files.push(files[i]);\n\
            }\n\
            else if (endsWith(files[i], \".js\"))\n\
            {\n\
                this.m_js_files.push(files[i]);\n\
            }\n\
        }\n\
    }\n\
    return CPageLoader;\n\
})();\n\
\n\
function Page_SetApplication(oMethod)\n\
{ \n\
    Page_Application = oMethod;\n\
}\n\
\n\
function Page_StartRefreshLoopRefresh()\n\
{\n\
    if(Page_RefreshMethod !== null &&\n\
            Page_RefreshPaused == false)\n\
    {\n\
        Page_RefreshMethod();\n\
    }\n\
}\n\
\n\
function Page_StartRefreshLoop(oMethod)\n\
{ \n\
    Page_RefreshMethod = oMethod;\n\
    Page_RefreshPaused = false;\n\
    if(Page_RefreshTimer == null)\n\
    {\n\
        Page_RefreshTimer = setInterval(Page_StartRefreshLoopRefresh, 3000);\n\
    }\n\
}\n\
\n\
function Page_PauseRefreshLoop()\n\
{\n\
    Page_RefreshPaused = true;\n\
}\n\
\n\
function Page_StopRefreshLoop()\n\
{\n\
    if(Page_RefreshTimer != null)\n\
    {\n\
        clearInterval(Page_RefreshTimer);\n\
        Page_RefreshTimer = null;\n\
    }\n\
}\n\
\n\
function Page_ConfirmLink(sQuery, sLink)\n\
{\n\
    if(confirm(sQuery))\n\
    {\n\
        window.location.href = sLink;\n\
    }\n\
}\n\
\n\
function Page_LoadMenu(sRestApiLink)\n\
{\n\
    var oAjax = new CAjax();\n\
    oAjax.onSuccess = function(oResult)\n\
    {\n\
        var oData = JSON.parse(oResult);\n\
        if(oData != null &&\n\
                oData.length != null &&\n\
                oData.length > 0)\n\
        {\n\
            var oMenu = Util_GetDivByIdOrCreate(\"menu\");\n\
            if(oMenu != null)\n\
            {\n\
                for(var i=0; i<oData.length; i++)\n\
                {\n\
                    var oEntry = document.createElement(\"span\");\n\
                    oEntry.innerHTML = oData[i].Name;\n\
                    oEntry.setAttribute('onclick', \"Page_LoadApplication('\"+oData[i].Link+\"')\");\n\
                    oMenu.appendChild(oEntry);\n\
                }\n\
            }\n\
        }\n\
    };\n\
    oAjax.get(sRestApiLink);\n\
}\n\
\n\
function Page_LoadFooter(sRestApiLink)\n\
{\n\
    var oAjax = new CAjax();\n\
    oAjax.onSuccess = function(oResult)\n\
    {\n\
        var oData = JSON.parse(oResult);\n\
        if(oData != null &&\n\
                oData.length != null &&\n\
                oData.length > 0)\n\
        {\n\
            var oFooter = Util_GetDivByIdOrCreate(\"footer\");\n\
            if(oFooter != null)\n\
            {\n\
                for(var i=0; i<oData.length; i++)\n\
                {\n\
                    var oEntry = document.createElement(\"span\");\n\
                    oEntry.innerHTML = oData[i].Name;\n\
                    oEntry.setAttribute('onclick', \"Page_LoadApplication('\"+oData[i].Link+\"')\");\n\
                    oFooter.appendChild(oEntry);\n\
                }\n\
            }\n\
        }\n\
    };\n\
    oAjax.get(sRestApiLink);\n\
}\n\
\n\
function Util_GetElementTypeByIdOrCreate(sType,sId)\n\
{\n\
    var oDiv = document.getElementById(sId);\n\
    if(oDiv == null)\n\
    {\n\
        oDiv = document.createElement(sType);\n\
        oDiv.id = sId;\n\
    }\n\
    return oDiv;\n\
}\n\
\n\
function Util_GetDivByIdOrCreate(sId){return Util_GetElementTypeByIdOrCreate(\"div\",sId);}\n\
\n\
function Util_UrlAppendParam(sUrl,sName,sValue)\n\
{\n\
    if(sUrl.includes('?'))\n\
    {\n\
        sUrl = sUrl + \"&\" + sName;\n\
    }\n\
    else\n\
    {\n\
        sUrl = sUrl + \"?\" + sName;\n\
    }\n\
    if(sValue != null)\n\
    {\n\
        sUrl = sUrl + \"=\" + sValue;\n\
    }\n\
    return sUrl;\n\
}\n\
\n\
function Util_RemoveAllChilds(oObject)\n\
{\n\
    if(oObject)\n\
    {\n\
        while (oObject.firstChild)\n\
        {\n\
            oObject.removeChild(oObject.firstChild);\n\
        }\n\
    }\n\
}\n\
\n\
";
size_t CcOSWebframework_Js_Length = 10232; 
