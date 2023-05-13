#ifndef webserver_h
#define webserver_h

const char index_html[] PROGMEM = R""""(
<!DOCTYPE html>
<html>
<head>
    <meta charset='utf-8'>
    <title>ESP32 Metrics Matrix</title>
    <style>
        body {
            font-family: Arial, Helvetica, sans-serif;
            background-color: #ffffff;
            color: #000000;
        }
        h1 {
            font-size: 1.5em;
            color: #ffffff;
            background-color: #000000;
            padding: 0.5em;
        }
        h2 {
            font-size: 1.2em;
            color: #000000;
            background-color: #ffffff;
            padding: 0.5em;
        }
        h3 {
            font-size: 1.0em;
            color: #000000;
            background-color: #ffffff;
            padding: 0.5em;
        }
        table {
            border-collapse: collapse;
            width: 100%%; // Escape the % sign with another % to work with template processing
        }
        th, td {
            text-align: left;
            padding: 0.25em;
        }
        tr:nth-child(even) {
            background-color: #dddddd;
        }
        .container {
            width: 100%%;
            max-width: 800px;
            margin: auto;
        }
        .footer {
            font-size: 0.8em;
            color: #ffffff;
            background-color: #000000;
            padding: 0.5em;
        }
    </style>
</head>
<body>
    <div class='container'>
        <h1>ESP32 LED Matrix</h1>
        <form method="POST" action="/set-query">
            <label for="label">Label: (optional, displays on second matrix if connected)</label><br>
            <input type="text" id="label" name="label" maxlength="10" size="10" value="%LABEL%"><br>
            <label for="unit">Unit: </label><br>
            <input type="text" id="unit" name="unit" maxlength="5" size="10" value="%UNIT%"><br>
            <label for="query">Query:</label><br>
            <input type="text" id="query" name="query" style="width: 90%%;" value="%QUERY%"><br><br>
            <label for="red">Red:</label><input type="number" id="red" name="red" min="0" max="255" size="3" value="%RED%">
            <label for="green">Green:</label><input type="number" id="green" name="green" min="0" max="255" size="3" value="%GREEN%">
            <label for="blue">Blue:</label><input type="number" id="blue" name="blue" min="0" max="255" size="3" value="%BLUE%"><br><br>
            <input type="submit" value="Submit">
        </form>
        <h3>Current Error</h3>
        %ERROR%
        <br><br>
        <div class='footer'>
            <p>esp32-metrics-matrix</p>
        </div>
    </div>
</body>
</html>)"""";

#endif
