#!/usr/bin/env python3

import cgi
import cgitb

# Enable debugging
cgitb.enable()

# Print HTTP headers
print("Content-Type: text/plain;charset=utf-8")
print()

# Get form data from the POST request
form = cgi.FieldStorage()

if form.getvalue('name') and form.getvalue('nickName') and form.getvalue('email'):
    # Retrieve form data
    name = form.getvalue('name')
    nickName = form.getvalue('nickName')
    email = form.getvalue('email')

    # Save data to a file
    with open("dataBase.txt", "a") as file:
        file.write(f"Name: {name} nickName: {nickName} email: {email}\n")

    print("Data saved successfully!")
else:
    print("Error: Required fields missing")