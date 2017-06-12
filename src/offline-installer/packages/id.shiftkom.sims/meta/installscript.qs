function Component() {
}

Component.prototype.createOperations = function() {
  component.createOperations();

  if (systemInfo.productType === "windows") {
    component.addOperation("CreateShortcut", "@TargetDir@/ShiftIMS.exe",
		                       "@StartMenuDir@/Shift IMS.lnk", "workingDirectory=@TargetDir@");
		component.addOperation("CreateShortcut", "@TargetDir@/ShiftIMS-Settings.exe",
                           "@StartMenuDir@/Shift IMS Settings.lnk", "workingDirectory=@TargetDir@");
		component.addOperation("CreateShortcut", "@TargetDir@/maintenancetool.exe",
		                       "@StartMenuDir@/Maintenance Tool.lnk", "workingDirectory=@TargetDir@");
  }
}
