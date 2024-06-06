import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

app.jinja_env.add_extension('jinja2.ext.loopcontrols')

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")

# default starting cash value defined in SQL database
""" !!!!!!! MUST BE CHANGED HERE MANUALLY IF ALTERED IN SQL !!!!!!! """
startingcash = 100000


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# generate current portfolio of logged in user from tx history
def generate_portfolio():

    # pull history of transactions of logged in user
    history = db.execute("SELECT * FROM tx WHERE user_id = ?", session["user_id"])
    portfolio = {}

    # sum shares from buy/sell txs
    for tx in history:

        # messy and infuriating solution to SQL being case insensitive
        tx["symbol"] = tx["symbol"].upper()

        if tx["symbol"] in portfolio.keys():
            portfolio[tx["symbol"]]["shares"] += tx["shares"]
        else:
            portfolio[tx["symbol"]] = {"shares" : tx["shares"]}

    # shear the stock out if its been sold off
    for stock in portfolio.copy():
        if portfolio[stock]["shares"] == 0:
            portfolio.pop(stock)
    # fill out rest of data from lookup() and calculate value of holdings
    sum = 0
    for stock in portfolio:
        quote = lookup(stock)
        portfolio[stock].update({
            "symbol" : quote["symbol"],
            "name" : quote["name"],
            "price" : quote["price"],
            "value" : quote["price"] * portfolio[stock]["shares"]
            })
        sum += portfolio[stock]["value"]
    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
    portfolio.update({"holdings" : sum, "cash" : cash, "total" : sum + cash, "profit" : (sum + cash) - startingcash})

    return portfolio


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    return render_template("index.html", portfolio=generate_portfolio())


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure all fields submitted
        if not request.form.get("symbol") or not request.form.get("shares"):
            return apology("you must provide stock symbol and number of shares", 666)

        # store inputs as vars
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        quote = lookup(symbol)

        # check if stock exists
        if quote == None:
            return apology("invalid stock symbol", 400)

        # ensure share amount is a positive integer
        if "." in shares or not shares.isdecimal() or int(shares) < 1:
            return apology("shares amount must be a positive integer", 400)

        # convert string to int
        shares = int(request.form.get("shares"))

        # check if they've got the cheddar

        user = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        if user[0]["cash"] < quote["price"] * shares:
            return apology("you absolutely cant afford that!", 666)

        # subtract cash for the buy
        db.execute("UPDATE users SET cash = (cash -  ?) WHERE id = ?", quote["price"] * shares, session["user_id"])

        # update transaction database
        db.execute("INSERT INTO tx (user_id, symbol, shares, price) VALUES (?, ?, ?, ?)", session["user_id"], symbol, shares, quote["price"])

        return redirect("/")
    else:

        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # pull history of transactions of logged in user
    history = db.execute("SELECT * FROM tx WHERE user_id = ?", session["user_id"])

    return render_template("history.html", history=history, portfolio=generate_portfolio())


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 667)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("symbol"):
            return apology("must provide stock symbol", 400)

        # store inputted symbol as a var
        symbol = request.form.get("symbol")

        quote = lookup(symbol)

        if quote == None:
            return apology("invalid stock symbol", 400)

        return render_template("quoted.html", quote=quote)

    else:
        # user reached this via GET so show input forms
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # password must not be username
        elif request.form.get("username") == request.form.get("password"):
            return apology("password must not be the same as username", 400)

        # Check that pwd and confirmation match
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords do not match", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # check if username already exists in database
        if len(rows) != 0:
            return apology("username already exists", 400)

        # insert new user into database
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", request.form.get("username"), generate_password_hash(request.form.get("password")))

        # return to login page on success
        return redirect("/login")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure all fields submitted
        if not request.form.get("symbol") or not request.form.get("shares") or request.form.get("symbol") == "Select a stock to sell":
            return apology("you must provide stock symbol and number of shares", 666)
        if request.form.get("shares") == "--":
            return apology("you must select a number of shares", 666)

        # store inputs as vars
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        quote = lookup(symbol)

        # check if stock exists
        if quote == None:
            return apology("invalid stock symbol. dont test my back end", 668)

        # ensure share amount is a positive integer
        if "." in shares or not shares.isdecimal() or int(shares) < 1:
            return apology("shares amount must be a positive integer", 400)

        # convert string to int
        shares = int(request.form.get("shares"))

        # check if they've got the shares
        portfolio = generate_portfolio()

        if symbol not in portfolio.keys():
            return apology("you dont own that stock. do not try and fuck with my back end", 666)

        if shares > portfolio[symbol]["shares"]:
            return apology("you dont have that many shares!", 400)

        # make shares amt negative to reflect sale
        shares = 0 - shares

        # add cash for the sell
        db.execute("UPDATE users SET cash = (cash -  ?) WHERE id = ?", quote["price"] * shares, session["user_id"])

        # update transaction database
        db.execute("INSERT INTO tx (user_id, symbol, shares, price) VALUES (?, ?, ?, ?)", session["user_id"], symbol, shares, quote["price"])

        return redirect("/")

    else:
        return render_template("sell.html", portfolio=generate_portfolio())
