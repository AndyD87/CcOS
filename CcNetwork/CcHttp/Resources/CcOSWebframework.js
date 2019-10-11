// Initialize gloabl page variables
var Page_Application = Page_LoadApplicationDefault;
var Page_RefreshPaused = false;
var Page_RefreshTimer  = null;
var Page_RefreshMethod = null;
var Page_RefreshPauseSymbol = null;

Page_ContentLoaded(window, Page_Init);

function Page_ToggleRefreshLoop()
{
    if(Page_RefreshMethod!=null)
    {
        Page_RefreshPaused = !Page_RefreshPaused;
        Page_RefreshPauseSymbolUpdate();
    }
}

function Page_RefreshPauseSymbolUpdate()
{
    if(Page_RefreshPaused)
    {
        if(Page_RefreshPauseSymbol) Page_RefreshPauseSymbol.innerHTML = '<b> &#x25BA; </b>';
    }
    else
    {
        if(Page_RefreshPauseSymbol) Page_RefreshPauseSymbol.innerHTML = '<b> &#x25A0; </b>';
    }
}

function Page_Init()
{
    if(Page_LoadFiles)
    {
        var oPageLoader = new CPageLoader(Page_LoadFiles);
        oPageLoader.loadFiles();
    }
    Page_Application();

    var oFooter = Util_GetDivByIdOrCreate("footer");
    Page_RefreshPauseSymbol = document.createElement("span");
    Page_RefreshPauseSymbol.setAttribute('onclick', "Page_ToggleRefreshLoop()");
    oFooter.appendChild(Page_RefreshPauseSymbol);
    Page_RefreshPauseSymbolUpdate();
}

function Page_ContentLoaded(win, fn)
{
    var done = false, top = true,
    doc = win.document,
    root = doc.documentElement,
    add = doc.addEventListener ? 'addEventListener' : 'attachEvent',
    rem = doc.addEventListener ? 'removeEventListener' : 'detachEvent',
    pre = doc.addEventListener ? '' : 'on',
    init = function(e)
    {
        if (e.type == 'readystatechange' && doc.readyState != 'complete') return;
        (e.type == 'load' ? win : doc)[rem](pre + e.type, init, false);
        if (!done && (done = true)) fn.call(win, e.type || e);
    },
    poll = function()
    {
        try { root.doScroll('left'); } catch(e) { setTimeout(poll, 50); return; }
        init('poll');
    };
    if (doc.readyState == 'complete') fn.call(win, 'lazy');
    else {
        if (doc.createEventObject && root.doScroll) {
            try { top = !win.frameElement; } catch(e) { }
            if (top) poll();
        }
        doc[add](pre + 'DOMPage_ContentLoaded', init, false);
        doc[add](pre + 'readystatechange', init, false);
        win[add](pre + 'load', init, false);
    }
}

function Page_LoadApplication(sRestApiLink)
{
    if( sRestApiLink === null ||
            !Page_Application(sRestApiLink))
    {
        Page_LoadApplicationDefault(sRestApiLink);
        Page_StartRefreshLoop( function() { Page_LoadApplicationDefault(sRestApiLink); });
    }
}

function Page_LoadApplicationDefault(sRestApiLink)
{
    if( sRestApiLink == null) sRestApiLink = '/api/app/status';
    if( sRestApiLink.startsWith('/api/app/status'))
    {
        var oAjax = new CAjax();
        var oResult = oAjax.get(sRestApiLink);
        oAjax.onSuccess = function(oResult)
        {
            var oData = JSON.parse(oResult);
            var oDevicesDiv = document.getElementById("content");
            if(oDevicesDiv !== null)
            {
                var oTable = document.createElement("table");
                for (var sKey in oData)
                {
                    var oRow = document.createElement("tr");
                    var oKey = document.createElement("td");
                    var oValue = document.createElement("td");
                    oKey.innerHTML = sKey;
                    oValue.innerHTML = oData[sKey];
                    oRow.appendChild(oKey);
                    oRow.appendChild(oValue);
                    oTable.appendChild(oRow);
                }
                oDevicesDiv.innerHTML = '';
                oDevicesDiv.appendChild(oTable);
            }
        };
        oAjax.get(sRestApiLink);
    }
}

function CAjax()
{
    var CAjax_this = this;
    this.get = function(sUrl)
    {
        if(CAjax.CurrentConnections < CAjax.MaxConnections)
        {
            CAjax.CurrentConnections++;
            var xhr = new XMLHttpRequest();
            xhr.open('GET', sUrl);
            xhr.onload = function()
            {
                CAjax.CurrentConnections--;
                if (xhr.status === 200)
                {
                    CAjax_this.onSuccess(xhr.responseText);
                }
                else
                {
                    CAjax_this.onError(xhr.status);
                }
                CAjax_this.sendNext();
            };
            xhr.send();
        }
        else
        {
            this.sUrl = sUrl;
            this.appendRequest(CAjax_this);
        }
    };
    this.post = function(sUrl, aDataArray)
    {
        var bFirst = 1;
        var sParam = '';
        for (var sKey in aDataArray)
        {
            if(bFirst == 1) bFirst = 0;
            else sParam += '&';
            sParam += sKey + '=' + aDataArray[sKey];
        }
        CAjax.CurrentConnections++;
        var xhr = new XMLHttpRequest();
        xhr.open('POST', sUrl, true);
        xhr.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
        xhr.onload = function()
        {
            CAjax.CurrentConnections--;
            if (xhr.status === 200)
            {
                CAjax_this.onSuccess(xhr.responseText);
            }
            else
            {
                CAjax_this.onError(xhr.status);
            }
        };
        xhr.send(sParam);
    };
    this.onSuccess = function(sData)
    {
    };
    this.onError = function(sData)
    {
        console.log('CAjax::onError:' + sData)
    };

    this.appendRequest = function(oAjax)
    {
        var bFound = 0;
        for(var i=0; i<CAjax.Requests.length; i++)
        {
            if(CAjax.Requests[i].sUrl === oAjax.sUrl)
            {
                bFound = true;
                break;
            }
        }
        if(bFound === 0)
        {
            CAjax.Requests.push(oAjax);
        }
    };

    this.sendNext = function()
    {
        if(CAjax.Requests.length)
        {
            var oAjax = CAjax.Requests.shift();
            oAjax.get(oAjax.sUrl);
        }
    };
}
CAjax.Requests              = [];
CAjax.MaxConnections        = 3;
CAjax.CurrentConnections    = 0;

var CPageLoader = (function ()
{
    function CPageLoader(files)
    {
        var _this = this;
        this.loadStyle = function (filename)
        {
            // HTMLLinkElement
            var link = document.createElement("link");
            link.rel = "stylesheet";
            link.type = "text/css";
            link.href = _filename;
            link.onload = function ()
            {
            };
            link.onerror = function ()
            {
            };
            _this.m_head.appendChild(link);
        };
        this.loadScript = function (filename)
        {
            var script = document.createElement('script');
            script.type = 'text/javascript';
            script.src = filename;
            script.onload = function ()
            {
            };
            script.onerror = function ()
            {
            };
            _this.m_head.appendChild(script);
        };
        this.loadFiles = function ()
        {
            for (var i = 0; i < _this.m_css_files.length; ++i)
                _this.loadStyle(_this.m_css_files[i]);
            for (var i = 0; i < _this.m_js_files.length; ++i)
                _this.loadScript(_this.m_js_files[i]);
        };
        this.m_js_files = [];
        this.m_css_files = [];
        this.m_head = document.getElementsByTagName("head")[0];
        function endsWith(str, suffix)
        {
            if (str === null || suffix === null)
                return false;
            return str.indexOf(suffix, str.length - suffix.length) !== -1;
        }
        for (var i = 0; i < files.length; ++i)
        {
            if (endsWith(files[i], ".css"))
            {
                this.m_css_files.push(files[i]);
            }
            else if (endsWith(files[i], ".js"))
            {
                this.m_js_files.push(files[i]);
            }
        }
    }
    return CPageLoader;
})();

function Page_SetApplication(oMethod)
{ 
    Page_Application = oMethod;
}

function Page_StartRefreshLoopRefresh()
{
    if(Page_RefreshMethod !== null &&
            Page_RefreshPaused == false)
    {
        Page_RefreshMethod();
    }
}

function Page_StartRefreshLoop(oMethod)
{ 
    Page_RefreshMethod = oMethod;
    Page_RefreshPaused = false;
    if(Page_RefreshTimer == null)
    {
        Page_RefreshTimer = setInterval(Page_StartRefreshLoopRefresh, 3000);
    }
}

function Page_PauseRefreshLoop()
{
    Page_RefreshPaused = true;
}

function Page_StopRefreshLoop()
{
    if(Page_RefreshTimer != null)
    {
        clearInterval(Page_RefreshTimer);
        Page_RefreshTimer = null;
    }
}

function Page_ConfirmLink(sQuery, sLink)
{
    if(confirm(sQuery))
    {
        window.location.href = sLink;
    }
}

function Page_LoadMenu(sRestApiLink)
{
    var oAjax = new CAjax();
    oAjax.onSuccess = function(oResult)
    {
        var oData = JSON.parse(oResult);
        if(oData != null &&
                oData.length != null &&
                oData.length > 0)
        {
            var oMenu = Util_GetDivByIdOrCreate("menu");
            if(oMenu != null)
            {
                for(var i=0; i<oData.length; i++)
                {
                    var oEntry = document.createElement("span");
                    oEntry.innerHTML = oData[i].Name;
                    oEntry.setAttribute('onclick', "Page_LoadApplication('"+oData[i].Link+"')");
                    oMenu.appendChild(oEntry);
                }
            }
        }
    };
    oAjax.get(sRestApiLink);
}

function Page_LoadFooter(sRestApiLink)
{
    var oAjax = new CAjax();
    oAjax.onSuccess = function(oResult)
    {
        var oData = JSON.parse(oResult);
        if(oData != null &&
                oData.length != null &&
                oData.length > 0)
        {
            var oFooter = Util_GetDivByIdOrCreate("footer");
            if(oFooter != null)
            {
                for(var i=0; i<oData.length; i++)
                {
                    var oEntry = document.createElement("span");
                    oEntry.innerHTML = oData[i].Name;
                    oEntry.setAttribute('onclick', "Page_LoadApplication('"+oData[i].Link+"')");
                    oFooter.appendChild(oEntry);
                }
            }
        }
    };
    oAjax.get(sRestApiLink);
}

function Util_GetElementTypeByIdOrCreate(sType,sId)
{
    var oDiv = document.getElementById(sId);
    if(oDiv == null)
    {
        oDiv = document.createElement(sType);
        oDiv.id = sId;
    }
    return oDiv;
}

function Util_GetDivByIdOrCreate(sId){return Util_GetElementTypeByIdOrCreate("div",sId);}

function Util_UrlAppendParam(sUrl,sName,sValue)
{
    if(sUrl.includes('?'))
    {
        sUrl = sUrl + "&" + sName;
    }
    else
    {
        sUrl = sUrl + "?" + sName;
    }
    if(sValue != null)
    {
        sUrl = sUrl + "=" + sValue;
    }
    return sUrl;
}

function Util_RemoveAllChilds(oObject)
{
    if(oObject)
    {
        while (oObject.firstChild)
        {
            oObject.removeChild(oObject.firstChild);
        }
    }
}

