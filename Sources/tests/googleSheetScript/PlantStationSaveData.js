function doPost(e) {
  var ss = SpreadsheetApp.getActiveSpreadsheet(); // Access the active spreadsheet
  var sheet = ss.getSheetByName("Sheet1"); // Access the sheet named "Sheet1"

  // Check if the sheet exists
  if (!sheet) {
    return ContentService.createTextOutput("Sheet not found").setMimeType(ContentService.MimeType.TEXT);
  }

  try {
    // Parse the incoming JSON data
    var data = JSON.parse(e.postData.contents);

    // Extract data fields with default values if missing
    var date = data.date || "N/A";
    var time = data.time || "N/A";
    var temperature = data.temperature || "N/A";
    var humidity = data.humidity || "N/A";
    var info = data.info || "N/A";

    // Append the extracted data to the sheet as a new row
    sheet.appendRow([date, time, temperature, humidity, info]);

    // Return a success response
    return ContentService.createTextOutput("Success").setMimeType(ContentService.MimeType.TEXT);
  } catch (error) {
    // Log and return error message if parsing or appending fails
    Logger.log("Error: " + error.message);
    return ContentService.createTextOutput("Error: " + error.message).setMimeType(ContentService.MimeType.TEXT);
  }
}
