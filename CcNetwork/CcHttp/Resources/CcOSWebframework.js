Page_ContentLoaded(window, Page_Init);

var Page_Application = function(sRestUrl){};
var Page_RefreshPaused = false;
var Page_RefreshTimer  = null;
var Page_RefreshMethod = null;

// from http://javascript.nwbox.com/Page_ContentLoaded/
function Page_ContentLoaded(win, fn) 
{
    var done = false, top = true,

    doc = win.document, root = doc.documentElement,

    add = doc.addEventListener ? 'addEventListener' : 'attachEvent',
    rem = doc.addEventListener ? 'removeEventListener' : 'detachEvent',
    pre = doc.addEventListener ? '' : 'on',

    init = function(e) {
        if (e.type == 'readystatechange' && doc.readyState != 'complete') return;
        (e.type == 'load' ? win : doc)[rem](pre + e.type, init, false);
        if (!done && (done = true)) fn.call(win, e.type || e);
    },

    poll = function() {
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

function Page_Init()
{
  //Wait for jquery
  if(window.jQuery == null)
  {
    setTimeout(function()
    {
      Page_Init();
    }, 100);
  }
  else
  {
    Page_Application();
  }
}

function Page_SetApplication(oMethod)
{ 
  Page_Application = oMethod;
}

function Page_StartRefreshLoop(oMethod)
{ 
  Page_RefreshMethod = oMethod;
  Page_RefreshPaused = false;
  if(Page_RefreshTimer == null)
  { 
    Page_RefreshMethod();
    Page_RefreshTimer = setInterval(Page_StartRefreshLoopRefresh, 3000);
  }
  
  function Page_StartRefreshLoopRefresh()
  {
    if(Page_RefreshPaused == false)
    {
      Page_RefreshMethod();
    }
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

function Page_CreateOverlay()
{
  if(document.body)
  {
    var oOverlayContainer = document.createElement('div');
    oOverlayContainer.classList.add("OverlayContainer");
    oOverlayContainer.id = "OverlayContainer";
    oOverlayContainer.onclick = Page_RemoveOverlay;

    oOverlayContent = document.createElement('div');
    oOverlayContent.classList.add("OverlayContent");
    oOverlayContent.id = "OverlayContent";
    oOverlayContent.onclick = Page_TriggerOverlay;

    var oSpinner = getSpinner();
    
    oOverlayContent.appendChild(oSpinner);
    oOverlayContainer.appendChild(oOverlayContent);
    document.body.appendChild(oOverlayContainer);
  }
}

function Page_TriggerOverlay(event)
{
  if(event != null)
  {
    event.stopPropagation();
  }
}

function Page_RemoveOverlay(event)
{
  var oOverlay = document.getElementById("OverlayContainer");
  if(oOverlay != null)
  {
    oOverlay.parentNode.removeChild(oOverlay);
  }
}

function Page_AutoresizeTextareas()
{
  jQuery.each(jQuery('textarea[data-autoresize]'), function() 
  {
    var offset = this.offsetHeight - this.clientHeight;
    var resizeTextarea = function(el) 
    {
      var uiOldHeight = el.offsetHeight;
      var uiNewHeight = el.scrollHeight + offset;
      if(uiNewHeight > uiOldHeight)
      {
        jQuery(el).css('height', 'auto').css('height', el.scrollHeight + offset);
      }
    };
    resizeTextarea(this);
    jQuery(this).on('keyup input', function() { resizeTextarea(this); }).removeAttr('data-autoresize');
  });
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
  // process the form
  $.get({
    url: sRestApiLink,
    success: function (sData)
    {
      var oData = JSON.parse(sData);
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
    }
  });
}

function Page_LoadFooter(sRestApiLink)
{
  // process the form
  $.get({
    url: sRestApiLink,
    success: function (sData)
    {
      var oData = JSON.parse(sData);
      if(oData != null &&
         oData.length != null &&
         oData.length > 0)
      {
        var oMenu = Util_GetDivByIdOrCreate("footer");
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
    }
  });
}

function Page_LoadApplication(sRestApiLink)
{
  Page_Application(sRestApiLink);
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

function Ajax_SendDiv(sDivName)
{
  var oTransferData = 
  {
	  'sMethod' : 'POST',
	  'sTarget' : window.location.href,
	  'sFormName' : sDivName+'_form',
	  'oDiv'    : document.getElementById(sDivName)
  };
  $('.output-shortcodes').find(':input');
  var f = document.createElement("form");
  f.setAttribute('id', oTransferData.sFormName);
  f.setAttribute('style', "display:none;");
  if(oTransferData.oDiv != null)
  {
    var oChilds = oTransferData.oDiv.querySelectorAll('input');
    for(var i=0; i<oChilds.length; i++)
    {
      if(oChilds[i].name == sDivName+'_method')
      {
    	  oTransferData.sMethod = oChilds[i].value;
      }
      else if(oChilds[i].name == sDivName+'_link')
      {
    	  oTransferData.sTarget = oChilds[i].value;
      }
      else
      {
        f.appendChild(oChilds[i]);
      }
    }
    var oChilds = oTransferData.oDiv.querySelectorAll('select')
    for(var i=0; i<oChilds.length; i++)
    {
      f.appendChild(oChilds[i]);
    }
    var oChilds = oTransferData.oDiv.querySelectorAll('textarea');
    for(var i=0; i<oChilds.length; i++)
    {
      f.appendChild(oChilds[i]);
    }
    // Remove all childs
    while (oTransferData.oDiv.firstChild) 
    {
    	oTransferData.oDiv.removeChild(oTransferData.oDiv.firstChild);
    }
    oTransferData.oDiv.appendChild(getSpinner());
    oTransferData.oDiv.appendChild(f);
    
    // stop the form from submitting the normal way and refreshing the page
    $('#'+sDivName+'_form').submit(oTransferData, function(event) 
    {
        event.preventDefault();
        var oForm = document.getElementById(event.data.sFormName);
        var oFormData = new FormData(oForm);    
        // process the form
        $.ajax({
          type: event.data.sMethod,
          url: event.data.sTarget,
          data: oFormData,
          enctype: "multipart/form-data",
          contentType: false,
          processData: false,
          cache: false,
          timeout: 60000,
          success: function (data)
          {
        	  event.data.oDiv.innerHTML = data;
            var oChilds = event.data.oDiv.querySelectorAll('input');
            for(var i=0; i<oChilds.length; i++)
            {
              if(oChilds[i].name == '__do_reload__')
              {
                setTimeout('location.reload()', oChilds[i].value);
              }
            }
          },
          error: function (data) 
          {
        	  event.data.oDiv.innerHTML = "ERROR";
          },
        });
    });
    //$('#'+sDivName+'_form').submit();
  }
}

function Ajax_Overlay(sUrl)
{
  $.get(
    sUrl,
    function( sData ) 
    {
      Page_CreateOverlay();
      var oOverlayContent = document.getElementById("OverlayContent");
      if(oOverlayContent != null)
      {
        oOverlayContent.innerHTML = sData;
      }
    })
    .done(function() {
      // additional success
    })
    .fail(function() {
      // error occured
    })
    .always(function() {
      // finished and always executed
    }
  );
}
