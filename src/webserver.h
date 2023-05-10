#ifndef webserver_h
#define webserver_h

#define TEMPLATE_PLACEHOLDER '`'

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
        <h3>Current Query</h3>
        %QUERY%
        <h3>New Query</h3>
        <form method="POST" action="/set-query">
            <label for="query">New Query:</label><br>
            <input type="text" id="query" name="query" style="width: 90%%;">
            <input type="submit" value="Submit">
        </form>
        <br><br>
        <div class='footer'>
            <p>esp32-metrics-matrix</p>
        </div>
    </div>
</body>
</html>)"""";

#endif
