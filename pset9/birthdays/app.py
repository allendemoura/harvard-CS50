import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":

        # TODO: Add the user's entry into the database
        name = request.form.get("name")
        month = request.form.get("month")
        day = request.form.get("day")
        if not (name or month or day):
            return render_template("fail.html")
        if not name.isalpha() or not month.isdigit() or not day.isdigit():
            return render_template("fail.html")
        if int(month) not in range(1,13) or int(day) not in range(1,32):
            return render_template("shorn.html")
        db.execute("INSERT INTO birthdays (name, month, day) VALUES (?, ?, ?)", name, month, day)

        return redirect("/")

    else:

        # TODO: Display the entries in the database on index.html

        birthdays = db.execute("SELECT * FROM birthdays")
        return render_template("index.html", birthdays=birthdays)


@app.route("/delete", methods=["POST"])
def delete():
    id = request.form.get("id")
    db.execute("DELETE FROM birthdays WHERE id = ?", id)
    return redirect("/")


@app.route("/edit", methods=["POST"])
def edit():
    id = request.form.get("id")
    name = request.form.get("newname")
    month = request.form.get("newmonth")
    day = request.form.get("newday")
    if not (name or month or day):
        return render_template("fail.html")
    if not name.isalpha() or not month.isdigit() or not day.isdigit():
        return render_template("fail.html")
    if int(month) not in range(1,13) or int(day) not in range(1,32):
        return render_template("shorn.html")
    db.execute("UPDATE birthdays SET name = ?, month = ?, day = ? WHERE id = ?", name, month, day, id)
    return redirect("/")
