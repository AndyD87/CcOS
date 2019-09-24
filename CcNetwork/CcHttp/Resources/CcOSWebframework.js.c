// Resource file generated from CcOSResource
#include "CcBase.h"
const char* CcOSWebframework_Js = "\
Page_ContentLoaded(window, Page_Init);\n\
\n\
var Page_Application = Page_LoadApplicationDefault;\n\
var Page_RefreshPaused = false;\n\
var Page_RefreshTimer  = null;\n\
var Page_RefreshMethod = null;\n\
\n\
// from http://javascript.nwbox.com/Page_ContentLoaded/\n\
function Page_ContentLoaded(win, fn) \n\
{\n\
    var done = false, top = true,\n\
\n\
    doc = win.document, root = doc.documentElement,\n\
\n\
    add = doc.addEventListener ? 'addEventListener' : 'attachEvent',\n\
    rem = doc.addEventListener ? 'removeEventListener' : 'detachEvent',\n\
    pre = doc.addEventListener ? '' : 'on',\n\
\n\
    init = function(e) {\n\
        if (e.type == 'readystatechange' && doc.readyState != 'complete') return;\n\
        (e.type == 'load' ? win : doc)[rem](pre + e.type, init, false);\n\
        if (!done && (done = true)) fn.call(win, e.type || e);\n\
    },\n\
\n\
    poll = function() {\n\
        try { root.doScroll('left'); } catch(e) { setTimeout(poll, 50); return; }\n\
        init('poll');\n\
    };\n\
\n\
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
function Page_Init()\n\
{\n\
  //Wait for jquery\n\
  if(window.jQuery == null)\n\
  {\n\
    setTimeout(function()\n\
    {\n\
      Page_Init();\n\
    }, 100);\n\
  }\n\
  else\n\
  {\n\
    Page_Application();\n\
  }\n\
}\n\
\n\
function Page_SetApplication(oMethod)\n\
{ \n\
  Page_Application = oMethod;\n\
}\n\
\n\
function Page_StartRefreshLoop(oMethod)\n\
{ \n\
  Page_RefreshMethod = oMethod;\n\
  Page_RefreshPaused = false;\n\
  if(Page_RefreshTimer == null)\n\
  { \n\
    Page_RefreshMethod();\n\
    Page_RefreshTimer = setInterval(Page_StartRefreshLoopRefresh, 3000);\n\
  }\n\
  \n\
  function Page_StartRefreshLoopRefresh()\n\
  {\n\
    if(Page_RefreshPaused == false)\n\
    {\n\
      Page_RefreshMethod();\n\
    }\n\
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
  { \n\
    clearInterval(Page_RefreshTimer);\n\
    Page_RefreshTimer = null;\n\
  }\n\
}\n\
\n\
function Page_CreateOverlay()\n\
{\n\
  if(document.body)\n\
  {\n\
    var oOverlayContainer = document.createElement('div');\n\
    oOverlayContainer.classList.add(\"OverlayContainer\");\n\
    oOverlayContainer.id = \"OverlayContainer\";\n\
    oOverlayContainer.onclick = Page_RemoveOverlay;\n\
\n\
    oOverlayContent = document.createElement('div');\n\
    oOverlayContent.classList.add(\"OverlayContent\");\n\
    oOverlayContent.id = \"OverlayContent\";\n\
    oOverlayContent.onclick = Page_TriggerOverlay;\n\
\n\
    var oSpinner = getSpinner();\n\
    \n\
    oOverlayContent.appendChild(oSpinner);\n\
    oOverlayContainer.appendChild(oOverlayContent);\n\
    document.body.appendChild(oOverlayContainer);\n\
  }\n\
}\n\
\n\
function Page_TriggerOverlay(event)\n\
{\n\
  if(event != null)\n\
  {\n\
    event.stopPropagation();\n\
  }\n\
}\n\
\n\
function Page_RemoveOverlay(event)\n\
{\n\
  var oOverlay = document.getElementById(\"OverlayContainer\");\n\
  if(oOverlay != null)\n\
  {\n\
    oOverlay.parentNode.removeChild(oOverlay);\n\
  }\n\
}\n\
\n\
function Page_AutoresizeTextareas()\n\
{\n\
  jQuery.each(jQuery('textarea[data-autoresize]'), function() \n\
  {\n\
    var offset = this.offsetHeight - this.clientHeight;\n\
    var resizeTextarea = function(el) \n\
    {\n\
      var uiOldHeight = el.offsetHeight;\n\
      var uiNewHeight = el.scrollHeight + offset;\n\
      if(uiNewHeight > uiOldHeight)\n\
      {\n\
        jQuery(el).css('height', 'auto').css('height', el.scrollHeight + offset);\n\
      }\n\
    };\n\
    resizeTextarea(this);\n\
    jQuery(this).on('keyup input', function() { resizeTextarea(this); }).removeAttr('data-autoresize');\n\
  });\n\
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
  // process the form\n\
  $.get({\n\
    url: sRestApiLink,\n\
    success: function (sData)\n\
    {\n\
      var oData = JSON.parse(sData);\n\
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
    }\n\
  });\n\
}\n\
\n\
function Page_LoadFooter(sRestApiLink)\n\
{\n\
  // process the form\n\
  $.get({\n\
    url: sRestApiLink,\n\
    success: function (sData)\n\
    {\n\
      var oData = JSON.parse(sData);\n\
      if(oData != null &&\n\
         oData.length != null &&\n\
         oData.length > 0)\n\
      {\n\
        var oMenu = Util_GetDivByIdOrCreate(\"footer\");\n\
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
    }\n\
  });\n\
}\n\
\n\
function Page_LoadApplication(sRestApiLink)\n\
{\n\
  if( sRestApiLink == null ||\n\
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
      $.get\n\
      (\n\
        sRestApiLink,\n\
        function( sData )\n\
        {\n\
          var oData = JSON.parse(sData);\n\
          var oDevicesDiv = document.getElementById(\"content\");\n\
          if(oDevicesDiv !== null)\n\
          {\n\
              oDevicesDiv.innerText = sData;\n\
          }\n\
        }\n\
      )\n\
      .done\n\
      (\n\
        function()\n\
        {\n\
          // additional success\n\
        }\n\
      )\n\
      .fail\n\
      (\n\
        function()\n\
        {\n\
          // error occured\n\
        }\n\
      )\n\
      .always\n\
      (\n\
        function()\n\
        {\n\
          // finished and always executed\n\
        }\n\
      );\n\
  }\n\
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
  {        \n\
    while (oObject.firstChild) \n\
    {\n\
      oObject.removeChild(oObject.firstChild);\n\
    }\n\
  }\n\
}\n\
\n\
function Ajax_SendDiv(sDivName)\n\
{\n\
  var oTransferData = \n\
  {\n\
    'sMethod' : 'POST',\n\
    'sTarget' : window.location.href,\n\
    'sFormName' : sDivName+'_form',\n\
    'oDiv'    : document.getElementById(sDivName)\n\
  };\n\
  $('.output-shortcodes').find(':input');\n\
  var f = document.createElement(\"form\");\n\
  f.setAttribute('id', oTransferData.sFormName);\n\
  f.setAttribute('style', \"display:none;\");\n\
  if(oTransferData.oDiv != null)\n\
  {\n\
    var oChilds = oTransferData.oDiv.querySelectorAll('input');\n\
    for(var i=0; i<oChilds.length; i++)\n\
    {\n\
      if(oChilds[i].name == sDivName+'_method')\n\
      {\n\
        oTransferData.sMethod = oChilds[i].value;\n\
      }\n\
      else if(oChilds[i].name == sDivName+'_link')\n\
      {\n\
        oTransferData.sTarget = oChilds[i].value;\n\
      }\n\
      else\n\
      {\n\
        f.appendChild(oChilds[i]);\n\
      }\n\
    }\n\
    var oChilds = oTransferData.oDiv.querySelectorAll('select')\n\
    for(var i=0; i<oChilds.length; i++)\n\
    {\n\
      f.appendChild(oChilds[i]);\n\
    }\n\
    var oChilds = oTransferData.oDiv.querySelectorAll('textarea');\n\
    for(var i=0; i<oChilds.length; i++)\n\
    {\n\
      f.appendChild(oChilds[i]);\n\
    }\n\
    // Remove all childs\n\
    while (oTransferData.oDiv.firstChild) \n\
    {\n\
      oTransferData.oDiv.removeChild(oTransferData.oDiv.firstChild);\n\
    }\n\
    oTransferData.oDiv.appendChild(getSpinner());\n\
    oTransferData.oDiv.appendChild(f);\n\
    \n\
    // stop the form from submitting the normal way and refreshing the page\n\
    $('#'+sDivName+'_form').submit(oTransferData, function(event) \n\
    {\n\
        event.preventDefault();\n\
        var oForm = document.getElementById(event.data.sFormName);\n\
        var oFormData = new FormData(oForm);    \n\
        // process the form\n\
        $.ajax({\n\
          type: event.data.sMethod,\n\
          url: event.data.sTarget,\n\
          data: oFormData,\n\
          enctype: \"multipart/form-data\",\n\
          contentType: false,\n\
          processData: false,\n\
          cache: false,\n\
          timeout: 60000,\n\
          success: function (data)\n\
          {\n\
            event.data.oDiv.innerHTML = data;\n\
            var oChilds = event.data.oDiv.querySelectorAll('input');\n\
            for(var i=0; i<oChilds.length; i++)\n\
            {\n\
              if(oChilds[i].name == '__do_reload__')\n\
              {\n\
                setTimeout('location.reload()', oChilds[i].value);\n\
              }\n\
            }\n\
          },\n\
          error: function (data) \n\
          {\n\
            event.data.oDiv.innerHTML = \"ERROR\";\n\
          },\n\
        });\n\
    });\n\
    //$('#'+sDivName+'_form').submit();\n\
  }\n\
}\n\
\n\
function Ajax_Overlay(sUrl)\n\
{\n\
  $.get(\n\
    sUrl,\n\
    function( sData ) \n\
    {\n\
      Page_CreateOverlay();\n\
      var oOverlayContent = document.getElementById(\"OverlayContent\");\n\
      if(oOverlayContent != null)\n\
      {\n\
        oOverlayContent.innerHTML = sData;\n\
      }\n\
    })\n\
    .done(function() {\n\
      // additional success\n\
    })\n\
    .fail(function() {\n\
      // error occured\n\
    })\n\
    .always(function() {\n\
      // finished and always executed\n\
    }\n\
  );\n\
}\n\
";
size_t CcOSWebframework_Js_Length = 9942; 
