#!/usr/bin/python
# Import modules for CGI handling
import cgi, cgitb
cgitb.enable()
# Create instance of FieldStorage
form = cgi.FieldStorage()

# Get data from fields
firstName = form.getvalue('first')
lastName  = form.getvalue('last')
print ("Content-type:text/html\r\n\r\n")
print ("<!DOCTYPE html>")


print ("<body class=\"stylito\">")
print ("<center>")
print ("<h1>Hello %s %s</h1>" % (firstName, lastName))
print ("<center>")
print ("</body>")
print ("</html>")
