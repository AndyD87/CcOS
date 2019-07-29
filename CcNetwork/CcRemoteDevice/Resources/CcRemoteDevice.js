
function DeviceGpioPin_PostToggle(sId, sUrl)
{
  // process the form
  $.post({
    url: sUrl,
    data: { 
        'method': 'toggle'
    },
    success: function (data)
    {
      DeviceGpioPin_Get(sId, sUrl);
    }
  });
}

function DeviceGpioPin_Get(sId, sUrl)
{
  $.get
  (
    sUrl,
    function( sData ) 
    {
      var oData = JSON.parse(sData);
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
          oCell.innerHTML = "Value:";
          
          oCell = document.createElement("td");
          oRow.appendChild(oCell);
          oCell.innerHTML = oData.Value;
          
          oCell = document.createElement("td");
          var oButton = document.createElement("input");
          oButton.type = "button";
          oButton.value = "toggle";
          oButton.onclick = function () { DeviceGpioPin_PostToggle(sId, sUrl); };
          oCell.appendChild(oButton);
          oRow.appendChild(oCell);
        }
      }
    }
  );
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
  if(!sUrl) sUrl = '/api/system/devices';
  Page_StartRefreshLoop( function() { CcRemoteDevice_GetDevices(sUrl); });
}

function CcRemoteDevice_GetDevices(sUrl)
{
  $.get
  (
    sUrl,
    function( sData ) 
    {
      var oData = JSON.parse(sData);
      var oDevicesDiv = document.getElementById("content");
      if(oDevicesDiv != null &&
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
            DeviceGpioPin_Get(oDevice.Id, sUrl + "/" + oDevice.Id);
            oDevicesDiv.appendChild(oDeviceDiv);
          }
          else
          {
            
          }
        }
      }
    }
  )
  .done
  (
    function() 
    {
      // additional success
    }
  )
  .fail
  (
    function() 
    {
      // error occured
    }
  )
  .always
  (
    function() 
    {
      // finished and always executed
    }
  );
}

Page_SetApplication(CcRemoteDevice_Loader);
