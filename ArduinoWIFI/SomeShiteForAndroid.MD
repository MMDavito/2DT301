
String shise = "POST /arduino_data HTTP/1.1\n";
shise += "Host: 85.24.161.85:5000\n";
shise += "Content-Type: multipart/form-data; boundary=----WebKitFormBoundary7MA4YWxkTrZu0gW\n";

shise += "----WebKitFormBoundary7MA4YWxkTrZu0gW\n";
shise += "Content-Disposition: form-data; name=\"Credentials\"\n";
shise += "\n";
shise += "ARDUINO_BAJS\n";
shise += "----WebKitFormBoundary7MA4YWxkTrZu0gW\n";
shise += "Content-Disposition: form-data; name=\"data\"\n";
shise += "\n";
shise += content;
shise += "\n";
shise += "----WebKitFormBoundary7MA4YWxkTrZu0gW\n";


String shise = "POST /arduino_data HTTP/1.1\n";
shise += "Host: 85.24.161.85:5000\n";
shise += "Content-Type: application/x-www-form-urlencoded\n";
shise += "\n";
shise += "Credentials=ARDUINO_BAJS&data=harrenMKristna666999er\n";
