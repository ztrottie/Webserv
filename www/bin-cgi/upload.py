#!/usr/bin/env python3
import os
import cgi
import cgitb

# Enable CGI debugging
cgitb.enable()

# Directory to store uploaded files
UPLOAD_DIR = './uploads'

# Ensure upload directory exists
os.makedirs(UPLOAD_DIR, exist_ok=True)

# Parse the form data
form = cgi.FieldStorage()

# Get the uploaded file
file_item = form['file']

# Get the filename provided in the request body
filename = file_item.filename
# Check if the file was uploaded and a filename is provided
if filename:
    # Ensure filename doesn't contain path traversal
    filename = os.path.basename(filename)

    # Construct the path to save the file
    file_path = os.path.join(UPLOAD_DIR, filename)

    # Open a file for writing
    with open(file_path, 'wb') as f:
        # Write the file contents to disk
        f.write(file_item.file.read())
    
    print("Content-Type: text/html\n")  # Print content type header
    print("<html><body>")
    print("<h2>File uploaded successfully.</h2>")
    print("<p>Filename: {}</p>".format(filename))
    print("<p>File saved to: {}</p>".format(file_path))
    print("</body></html>")
else:
    print("Content-Type: text/html\n")  # Print content type header
    print("<html><body>")
    print("<h2>Error: No file uploaded or filename provided.</h2>")
    print("</body></html>")
