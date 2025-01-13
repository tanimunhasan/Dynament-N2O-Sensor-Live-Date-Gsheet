function doPost(e) {
  Logger.log(JSON.stringify(e)); // Log the entire event object
  
  var sheet = SpreadsheetApp.getActiveSpreadsheet().getActiveSheet();
  
  if (!e.parameter.gasReading) {
    return ContentService.createTextOutput("Error: Missing gasReading parameter");
  }
  
  var gasReading = e.parameter.gasReading;
  sheet.appendRow([new Date(), gasReading]);
  
  return ContentService.createTextOutput("Data received successfully");
}
