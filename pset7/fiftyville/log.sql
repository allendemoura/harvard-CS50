-- | Ruth    | Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot
-- and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that
-- left the parking lot in that time frame.
-- | Eugene  | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I
-- arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.
-- | Raymond | As the thief was leaving the bakery, they called someone who talked to them for less than a minute.
-- In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
-- The thief then asked the person on the other end of the phone to purchase the flight ticket.
-- find crime report. mentions 3 witnesses, bakery, and 10:15am
SELECT
    *
FROM
    crime_scene_reports
WHERE
    street = 'Humphrey Street'
    AND year = 2021
    AND MONTH = 7
    AND DAY = 28;

-- query the 3 interviews mentioned in crime report
SELECT
    name,
    transcript
FROM
    interviews
WHERE
    transcript LIKE '%bakery%'
    AND year = 2021
    AND MONTH = 7
    AND DAY = 28;

-- following up on first interview lead, querying list of lisence plates that exited within 10 minutes of crime
-- yields 8 plates
SELECT
    license_plate
FROM
    bakery_security_logs
WHERE
    year = 2021
    AND MONTH = 7
    AND DAY = 28
    AND HOUR = 10
    AND MINUTE >= 5
    AND MINUTE <= 25
    AND activity = 'exit';

-- following up on second lead, querying atm records for license plates of people who made withdrawals on crime day
-- before the crime time
SELECT
    license_plate
FROM
    atm_transactions
    JOIN bank_accounts ON atm_transactions.account_number = bank_accounts.account_number
    JOIN people ON bank_accounts.person_id = people.id
WHERE
    atm_location = 'Leggett Street'
    AND year = 2021
    AND MONTH = 7
    AND DAY = 28
    AND transaction_type = 'withdraw';

-- correlating first and second leads for matches, yields 4 license plates that were in both places at the witness specified times
SELECT
    license_plate
FROM
    bakery_security_logs
WHERE
    year = 2021
    AND MONTH = 7
    AND DAY = 28
    AND HOUR = 10
    AND MINUTE >= 5
    AND MINUTE <= 25
    AND activity = 'exit'
    AND license_plate IN (
        SELECT
            license_plate
        FROM
            atm_transactions
            JOIN bank_accounts ON atm_transactions.account_number = bank_accounts.account_number
            JOIN people ON bank_accounts.person_id = people.id
        WHERE
            atm_location = 'Leggett Street'
            AND year = 2021
            AND MONTH = 7
            AND DAY = 28
            AND transaction_type = 'withdraw'
    );

-- look up all info of the people associated with each license plate. these are 4 prime suspects
SELECT
    *
FROM
    people
WHERE
    license_plate IN (
        SELECT
            license_plate
        FROM
            bakery_security_logs
        WHERE
            year = 2021
            AND MONTH = 7
            AND DAY = 28
            AND HOUR = 10
            AND MINUTE >= 5
            AND MINUTE <= 25
            AND activity = 'exit'
            AND license_plate IN (
                SELECT
                    license_plate
                FROM
                    atm_transactions
                    JOIN bank_accounts ON atm_transactions.account_number = bank_accounts.account_number
                    JOIN people ON bank_accounts.person_id = people.id
                WHERE
                    atm_location = 'Leggett Street'
                    AND year = 2021
                    AND MONTH = 7
                    AND DAY = 28
                    AND transaction_type = 'withdraw'
            )
    );

-- create new table modeled after people table with just these suspects to save me from disgusting level of nested selects
CREATE TABLE suspects (
    id INTEGER,
    name TEXT,
    phone_number TEXT,
    passport_number INTEGER,
    license_plate TEXT,
    PRIMARY KEY(id)
);

-- populate table
INSERT INTO
    suspects
SELECT
    *
FROM
    people
WHERE
    license_plate IN (
        SELECT
            license_plate
        FROM
            bakery_security_logs
        WHERE
            year = 2021
            AND MONTH = 7
            AND DAY = 28
            AND HOUR = 10
            AND MINUTE >= 5
            AND MINUTE <= 25
            AND activity = 'exit'
            AND license_plate IN (
                SELECT
                    license_plate
                FROM
                    atm_transactions
                    JOIN bank_accounts ON atm_transactions.account_number = bank_accounts.account_number
                    JOIN people ON bank_accounts.person_id = people.id
                WHERE
                    atm_location = 'Leggett Street'
                    AND year = 2021
                    AND MONTH = 7
                    AND DAY = 28
                    AND transaction_type = 'withdraw'
            )
    );

-- follow up on third lead, query for suspects who made phone calls around time of crime, yields 2 suspects
SELECT
    *
FROM
    suspects
WHERE
    phone_number IN (
        SELECT
            caller
        FROM
            phone_calls
        WHERE
            year = 2021
            AND MONTH = 7
            AND DAY = 28
            AND duration < 60
    );

-- update suspects to save trouble
DELETE FROM
    suspects
WHERE
    NOT name = 'Bruce'
    AND NOT name = 'Diana';

-- get numbers of recievers of these 2 calls
SELECT
    receiver
FROM
    suspects
    JOIN phone_calls ON phone_number = caller
WHERE
    suspects.phone_number = phone_calls.caller
    AND year = 2021
    AND MONTH = 7
    AND DAY = 28
    AND duration < 60;

-- found that 50ville port is id=8
SELECT
    *
FROM
    airports;

-- find id of first flight out of fiftyville the day after crime as mentioned in testimony. its 36
SELECT
    id
FROM
    flights
WHERE
    origin_airport_id = 8
    AND year = 2021
    AND MONTH = 7
    AND DAY = 29
ORDER BY
    HOUR
LIMIT
    1;

-- see which suspect is on flight 36. its Bruce!!
SELECT
    name
FROM
    passengers
    JOIN suspects ON passengers.passport_number = suspects.passport_number
WHERE
    flight_id = 36;

-- look at where flight is going
SELECT
    destination_airport_id
FROM
    flights
WHERE
    id = 36;

-- id = 4, check airports table for visual ref. the dirty bastard's going to the Big Apple!
SELECT
    *
FROM
    airports
WHERE
    id = 4;

-- bruce called (375) 555-8161 earlier, lets see who it is?!
SELECT
    *
FROM
    people
WHERE
    phone_number = '(375) 555-8161';

-- Robin is the accomplice and he doesnt even have a passport! guess you wont be fleeing the scene then bud.