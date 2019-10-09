// Resource file generated from CcOSResource
#include "CcBase.h"
const char* CcRemoteDevice_Js = "\
function CcRemoteDevice_GpioPin_PostToggle(sId, sUrl)\n\
{\n\
    var oAjax = new CAjax();\n\
    oAjax.onSuccess = function(oResult)\n\
    {\n\
        CcRemoteDevice_GpioPin_Get(sId, sUrl);\n\
    };\n\
    oAjax.post(sUrl, {'method':'toggle'});\n\
}\n\
\n\
function CcRemoteDevice_GpioPin_Get(sId, sUrl)\n\
{\n\
    var oAjax = new CAjax();\n\
    oAjax.onSuccess = function(oResult)\n\
    {\n\
        var oData = JSON.parse(oResult);\n\
        var oDeviceDiv = document.getElementById(\"device_\"+sId);\n\
        if(oDeviceDiv != null &&\n\
                oData != null &&\n\
                oData.Type == \"GpioPin\")\n\
        {\n\
            Util_RemoveAllChilds(oDeviceDiv);\n\
            oDeviceDiv.classList.add(\"gpio\");\n\
            var oName  = document.createElement(\"h3\");\n\
            oDeviceDiv.appendChild(oName);\n\
            oName.innerHTML = oData.Name;\n\
\n\
            var oTable = null;\n\
            var oRow   = null;\n\
            var oCell  = null;\n\
            oTable = document.createElement(\"table\");\n\
            oDeviceDiv.appendChild(oTable);\n\
\n\
            oRow   = document.createElement(\"tr\");\n\
            oCell  = document.createElement(\"td\");\n\
            oTable.appendChild(oRow);\n\
            oRow.appendChild(oCell);\n\
            oCell.innerHTML = \"Name:\";\n\
            oCell = document.createElement(\"td\");\n\
            oRow.appendChild(oCell);\n\
            oCell.innerHTML = oData.Name;\n\
            oCell.colSpan = 2;\n\
\n\
            if(oData.Id != null)\n\
            {\n\
                oRow   = document.createElement(\"tr\");\n\
\n\
                oCell  = document.createElement(\"td\");\n\
                oTable.appendChild(oRow);\n\
                oRow.appendChild(oCell);\n\
                oCell.innerHTML = \"Id:\";\n\
\n\
                oCell = document.createElement(\"td\");\n\
                oRow.appendChild(oCell);\n\
                oCell.innerHTML = oData.Id;\n\
                oCell.colSpan = 2;\n\
            }\n\
            if(oData.Value != null)\n\
            {\n\
                oRow   = document.createElement(\"tr\");\n\
                oCell  = document.createElement(\"td\");\n\
                oTable.appendChild(oRow);\n\
                oRow.appendChild(oCell);\n\
                oCell.innerHTML = \"Value:\";\n\
\n\
                oCell = document.createElement(\"td\");\n\
                oRow.appendChild(oCell);\n\
                oCell.innerHTML = oData.Value;\n\
\n\
                oCell = document.createElement(\"td\");\n\
                var oButton = document.createElement(\"input\");\n\
                oButton.type = \"button\";\n\
                oButton.value = \"toggle\";\n\
                oButton.onclick = function () { CcRemoteDevice_GpioPin_PostToggle(sId, sUrl); };\n\
                oCell.appendChild(oButton);\n\
                oRow.appendChild(oCell);\n\
            }\n\
        }\n\
    };\n\
    oAjax.get(sUrl);\n\
}\n\
\n\
function CcRemoteDevice_Network_Get(sId, sUrl)\n\
{\n\
    var oAjax = new CAjax();\n\
    oAjax.onSuccess = function(oResult)\n\
    {\n\
        var oData = JSON.parse(oResult);\n\
        var oDeviceDiv = document.getElementById(\"device_\"+sId);\n\
        if(oDeviceDiv != null &&\n\
                oData != null &&\n\
                oData.Type == \"Network\")\n\
        {\n\
            Util_RemoveAllChilds(oDeviceDiv);\n\
            oDeviceDiv.classList.add(\"gpio\");\n\
            var oName  = document.createElement(\"h3\");\n\
            oDeviceDiv.appendChild(oName);\n\
            oName.innerHTML = oData.Name;\n\
\n\
            var oTable = null;\n\
            var oRow   = null;\n\
            var oCell  = null;\n\
            oTable = document.createElement(\"table\");\n\
            oDeviceDiv.appendChild(oTable);\n\
\n\
            oRow   = document.createElement(\"tr\");\n\
            oCell  = document.createElement(\"td\");\n\
            oTable.appendChild(oRow);\n\
            oRow.appendChild(oCell);\n\
            oCell.innerHTML = \"Name:\";\n\
            oCell = document.createElement(\"td\");\n\
            oRow.appendChild(oCell);\n\
            oCell.innerHTML = oData.Name;\n\
            oCell.colSpan = 2;\n\
\n\
            if(oData.Id != null)\n\
            {\n\
                oRow   = document.createElement(\"tr\");\n\
\n\
                oCell  = document.createElement(\"td\");\n\
                oTable.appendChild(oRow);\n\
                oRow.appendChild(oCell);\n\
                oCell.innerHTML = \"Id:\";\n\
\n\
                oCell = document.createElement(\"td\");\n\
                oRow.appendChild(oCell);\n\
                oCell.innerHTML = oData.Id;\n\
                oCell.colSpan = 2;\n\
            }\n\
            if(oData.Value != null)\n\
            {\n\
                oRow   = document.createElement(\"tr\");\n\
                oCell  = document.createElement(\"td\");\n\
                oTable.appendChild(oRow);\n\
                oRow.appendChild(oCell);\n\
                oCell.innerHTML = \"Value:\";\n\
\n\
                oCell = document.createElement(\"td\");\n\
                oRow.appendChild(oCell);\n\
                oCell.innerHTML = oData.Value;\n\
\n\
                oCell = document.createElement(\"td\");\n\
                var oButton = document.createElement(\"input\");\n\
                oButton.type = \"button\";\n\
                oButton.value = \"toggle\";\n\
                oButton.onclick = function () { CcRemoteDevice_GpioPin_PostToggle(sId, sUrl); };\n\
                oCell.appendChild(oButton);\n\
                oRow.appendChild(oCell);\n\
            }\n\
        }\n\
    };\n\
    oAjax.get(sUrl);\n\
}\n\
\n\
var CcRemoteDevice_Globals =\n\
        {\n\
    'content' :\n\
    {\n\
        'url' : ''\n\
    }\n\
};\n\
\n\
function CcRemoteDevice_Loader(sUrl)\n\
{\n\
    var bSuccess = false;\n\
    if(!sUrl)\n\
    {\n\
        // Stop processing\n\
    }\n\
    else if(sUrl.startsWith('/api/system'))\n\
    {\n\
        bSuccess = true;\n\
        CcRemoteDevice_GetDevices(sUrl);\n\
        Page_StartRefreshLoop( function() { CcRemoteDevice_GetDevices(sUrl); });\n\
    }\n\
    return bSuccess;\n\
}\n\
\n\
function CcRemoteDevice_GetDevices(sUrl)\n\
{\n\
    var oAjax = new CAjax();\n\
    oAjax.onSuccess = function(oResult)\n\
    {\n\
        var oData = JSON.parse(oResult);\n\
        var oDevicesDiv = document.getElementById(\"content\");\n\
        if(oDevicesDiv != null &&\n\
                oData.Devices != null)\n\
        {\n\
            var sLastDevice = '';\n\
            var bRefresh = false;\n\
            if(CcRemoteDevice_Globals.content.url != sUrl)\n\
            {\n\
                CcRemoteDevice_Globals.content.url = sUrl\n\
                Util_RemoveAllChilds(oDevicesDiv);\n\
                bRefresh = true;\n\
            }\n\
            for(var i=0; i < oData.Devices.length; i++)\n\
            {\n\
                var oDevice = oData.Devices[i]\n\
                var oDeviceDiv = Util_GetDivByIdOrCreate(\"device_\"+oDevice.Id)\n\
                oDeviceDiv.classList.add(\"device\");\n\
                if(oDevice.Type != sLastDevice && bRefresh)\n\
                {\n\
                    var oHeader = Util_GetElementTypeByIdOrCreate(\"h2\", \"device_head_\"+oDevice.Type);\n\
                    oHeader.innerText = oDevice.Type;\n\
                    sLastDevice = oDevice.Type;\n\
                    oDevicesDiv.appendChild(oHeader);\n\
                }\n\
                if(oDevice.Type == \"GpioPin\")\n\
                {\n\
                    CcRemoteDevice_GpioPin_Get(oDevice.Id, sUrl + \"/\" + oDevice.Id);\n\
                    oDevicesDiv.appendChild(oDeviceDiv);\n\
                }\n\
                else if(oDevice.Type == \"Network\")\n\
                {\n\
\n\
                }\n\
\n\
                else\n\
                {\n\
\n\
                }\n\
            }\n\
        }\n\
    }\n\
    oAjax.get(sUrl);\n\
}\n\
\n\
Page_SetApplication(CcRemoteDevice_Loader);\n\
";
size_t CcRemoteDevice_Js_Length = 7314; 
