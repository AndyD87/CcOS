function CcRemoteDevice_GpioPin_PostToggle(sId, sUrl)
{
    var oAjax = new CAjax();
    oAjax.onSuccess = function(oResult)
    {
        CcRemoteDevice_GpioPin_Get(sId, sUrl);
    };
    oAjax.post(sUrl, {'method':'toggle'});
}

function CcRemoteDevice_GpioPin_Get(sId, sUrl)
{
    var oAjax = new CAjax();
    oAjax.onSuccess = function(oResult)
    {
        var oData = JSON.parse(oResult);
        CcRemoteDevice_GpioPin_Parse(sId, oData, sUrl);
    };
    oAjax.get(sUrl);
}

function CcRemoteDevice_GpioPin_Parse(sId, oData, sUrl)
{
    var oDeviceDiv = document.getElementById("device_"+sId);
    if(oDeviceDiv != null &&
            oData != null &&
            oData.Type == "GpioPin")
    {
        Util_RemoveAllChilds(oDeviceDiv);
        oDeviceDiv.classList.add("gpio");
        var oName  = document.createElement("h3");
        oDeviceDiv.appendChild(oName);
        oName.innerHTML = oData.Name;

        var oTable = null;
        var oRow   = null;
        var oCell  = null;
        oTable = document.createElement("table");
        oDeviceDiv.appendChild(oTable);

        oRow   = document.createElement("tr");
        oCell  = document.createElement("td");
        oTable.appendChild(oRow);
        oRow.appendChild(oCell);
        oCell.innerHTML = "Name:";
        oCell = document.createElement("td");
        oRow.appendChild(oCell);
        oCell.innerHTML = oData.Name;
        oCell.colSpan = 2;

        if(oData.Id != null)
        {
            oRow   = document.createElement("tr");

            oCell  = document.createElement("td");
            oTable.appendChild(oRow);
            oRow.appendChild(oCell);
            oCell.innerHTML = "Id:";

            oCell = document.createElement("td");
            oRow.appendChild(oCell);
            oCell.innerHTML = oData.Id;
            oCell.colSpan = 2;
        }
        if(oData.Value != null)
        {
            oRow   = document.createElement("tr");
            oCell  = document.createElement("td");
            oTable.appendChild(oRow);
            oRow.appendChild(oCell);
            oCell.innerHTML = "Direction:";

            oCell = document.createElement("td");
            oRow.appendChild(oCell);
            switch(oData.Direction)
            {
                case 0:
                    oCell.innerHTML = "Unknown";
                    break;
                case 1:
                    oCell.innerHTML = "Input";
                    break;
                case 2:
                    oCell.innerHTML = "Output";
                    break;
                case 3:
                    oCell.innerHTML = "Analog";
                    break;
                default:
                    oCell.innerHTML = "Alternate(" + (oData.Direction-4) + ")";
                    break;

            }

            oRow   = document.createElement("tr");
            oCell  = document.createElement("td");
            oTable.appendChild(oRow);
            oRow.appendChild(oCell);
            oCell.innerHTML = "Value:";

            oCell = document.createElement("td");
            oRow.appendChild(oCell);
            oCell.innerHTML = oData.Value;

            oCell = document.createElement("td");
            var oButton = document.createElement("input");
            oButton.type = "button";
            oButton.value = "toggle";
            oButton.onclick = function () { CcRemoteDevice_GpioPin_PostToggle(sId, sUrl); };
            oCell.appendChild(oButton);
            oRow.appendChild(oCell);
        }
    }
}

function CcRemoteDevice_Network_Get(sId, sUrl)
{
    var oAjax = new CAjax();
    oAjax.onSuccess = function(oResult)
    {
        var oData = JSON.parse(oResult);
        var oDeviceDiv = document.getElementById("device_"+sId);
        if(oDeviceDiv != null &&
                oData != null &&
                oData.Type == "Network")
        {
            Util_RemoveAllChilds(oDeviceDiv);
            oDeviceDiv.classList.add("gpio");
            var oName  = document.createElement("h3");
            oDeviceDiv.appendChild(oName);
            oName.innerHTML = oData.Name;

            var oTable = null;
            var oRow   = null;
            var oCell  = null;
            oTable = document.createElement("table");
            oDeviceDiv.appendChild(oTable);

            oRow   = document.createElement("tr");
            oCell  = document.createElement("td");
            oTable.appendChild(oRow);
            oRow.appendChild(oCell);
            oCell.innerHTML = "Name:";
            oCell = document.createElement("td");
            oRow.appendChild(oCell);
            oCell.innerHTML = oData.Name;
            oCell.colSpan = 2;

            if(oData.Id != null)
            {
                oRow   = document.createElement("tr");

                oCell  = document.createElement("td");
                oTable.appendChild(oRow);
                oRow.appendChild(oCell);
                oCell.innerHTML = "Id:";

                oCell = document.createElement("td");
                oRow.appendChild(oCell);
                oCell.innerHTML = oData.Id;
                oCell.colSpan = 2;
            }
            if(oData.Value != null)
            {
                oRow   = document.createElement("tr");
                oCell  = document.createElement("td");
                oTable.appendChild(oRow);
                oRow.appendChild(oCell);
                oCell.innerHTML = "Value:";

                oCell = document.createElement("td");
                oRow.appendChild(oCell);
                oCell.innerHTML = oData.Value;

                oCell = document.createElement("td");
                var oButton = document.createElement("input");
                oButton.type = "button";
                oButton.value = "toggle";
                oButton.onclick = function () { CcRemoteDevice_GpioPin_PostToggle(sId, sUrl); };
                oCell.appendChild(oButton);
                oRow.appendChild(oCell);
            }
        }
    };
    oAjax.get(sUrl);
}

var CcRemoteDevice_Globals =
        {
    'content' :
    {
        'url' : ''
    }
};

function CcRemoteDevice_Loader(sUrl)
{
    var bSuccess = false;
    if(!sUrl)
    {
        // Stop processing
    }
    else if(sUrl.startsWith('/api/system'))
    {
        bSuccess = true;
        CcRemoteDevice_GetDevices(sUrl);
        Page_StartRefreshLoop( function() { CcRemoteDevice_GetDevices(sUrl); });
    }
    return bSuccess;
}

function CcRemoteDevice_GetDevices(sUrl)
{
    var oAjax = new CAjax();
    oAjax.onSuccess = function(oResult)
    {
        var oData = JSON.parse(oResult);
        var oDevicesDiv = document.getElementById("content");
        if( oDevicesDiv != null &&
            oData.Devices != null)
        {
            var sLastDevice = '';
            var bRefresh = false;
            if(CcRemoteDevice_Globals.content.url != sUrl)
            {
                CcRemoteDevice_Globals.content.url = sUrl
                Util_RemoveAllChilds(oDevicesDiv);
                bRefresh = true;
            }
            for(var i=0; i < oData.Devices.length; i++)
            {
                var oDevice = oData.Devices[i]
                var oDeviceDiv = Util_GetDivByIdOrCreate("device_"+oDevice.Id)
                oDeviceDiv.classList.add("device");
                if(oDevice.Type != sLastDevice && bRefresh)
                {
                    var oHeader = Util_GetElementTypeByIdOrCreate("h2", "device_head_"+oDevice.Type);
                    oHeader.innerText = oDevice.Type;
                    sLastDevice = oDevice.Type;
                    oDevicesDiv.appendChild(oHeader);
                }
                if(oDevice.Type == "GpioPin")
                {
                    CcRemoteDevice_GpioPin_Parse(oDevice.Id, oDevice, sUrl + "/" + oDevice.Id);
                    oDevicesDiv.appendChild(oDeviceDiv);
                }
                else if(oDevice.Type == "Network")
                {

                }

                else
                {

                }
            }
        }
    }
    oAjax.get(sUrl);
}

Page_SetApplication(CcRemoteDevice_Loader);
