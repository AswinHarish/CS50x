
-- For viewing report of crime that happened on July 28, 2021 at Humphrey Street.
SELECT * FROM crime_scene_reports WHERE year = 2021 AND month = 7 AND day = 28 AND street = 'Humphrey Street';

-- Date: July 28, 2021
-- Place: Humphrey Street
-- Time : 10:15am

-- Three witnesses mentioned about bakery in interview
-- Need to cheak their statements for more details
SELECT * FROM interviews WHERE year = 2021 AND month = 7 AND day = 28;

-- Things to note from interview
-- A Car left within 10 minutes of the theft from bakery parking lot (between 10:15 and 10:25)
-- (Name: Eugene, id: 162 )| Recognized the thief as he saw the thief withdrawin at the atm on Leggett Street
-- Thief called someone when leaving the bakery and talked to someone on phone for less than a minute and asked to purchase ticket for earliest flight out of fiftyville


-- Checking exit activity in bakery security log of July 28 2021 between 10:15am and 10:25 am
SELECT * FROM bakery_security_logs WHERE  year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute >= 15 AND minute <= 25 AND activity = 'exit';
-- 8 Cars left between this time range had left the place. One of this is the thief
-- +---------------+
-- | license_plate |
-- +---------------+
-- | 5P2BI95       |
-- | 94KL13X       |
-- | 6P58WS2       |
-- | 4328GD8       |
-- | G412CB7       |
-- | L93JTIZ       |
-- | 322W7JE       |
-- | 0NTHK55       |
-- +---------------+

-- We need to check the flight ticket bookings for next day, Phone call logs less than a minute and atm transactions of the current day at Leggett Street
SELECT * FROM atm_transactions WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = 'Leggett Street';
-- Found 9 account numbers which tranasacted on the date at the specific atm
-- +----------------+
-- | account_number |
-- +----------------+
-- | 28500762       |
-- | 28296815       |
-- | 76054385       |
-- | 49610011       |
-- | 16153065       |
-- | 86363979       |
-- | 25506511       |
-- | 81061156       |
-- | 26013199       |
-- +----------------+

SELECT * FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60;
-- 9 phone calls which lasted less than a minute
-- +-----+----------------+----------------+------+-------+-----+----------+
-- | id  |     caller     |    receiver    | year | month | day | duration |
-- +-----+----------------+----------------+------+-------+-----+----------+
-- | 221 | (130) 555-0289 | (996) 555-8899 | 2021 | 7     | 28  | 51       |
-- | 224 | (499) 555-9472 | (892) 555-8872 | 2021 | 7     | 28  | 36       |
-- | 233 | (367) 555-5533 | (375) 555-8161 | 2021 | 7     | 28  | 45       |
-- | 251 | (499) 555-9472 | (717) 555-1342 | 2021 | 7     | 28  | 50       |
-- | 254 | (286) 555-6063 | (676) 555-6554 | 2021 | 7     | 28  | 43       |
-- | 255 | (770) 555-1861 | (725) 555-3243 | 2021 | 7     | 28  | 49       |
-- | 261 | (031) 555-6622 | (910) 555-3251 | 2021 | 7     | 28  | 38       |
-- | 279 | (826) 555-1652 | (066) 555-9701 | 2021 | 7     | 28  | 55       |
-- | 281 | (338) 555-6650 | (704) 555-2131 | 2021 | 7     | 28  | 54       |
-- +-----+----------------+----------------+------+-------+-----+----------+



SELECT * FROM flights WHERE year = 2021 AND month = 7 AND day = 29 ORDER BY hour ASC LIMIT 1;
-- Earliest flight out of fiftyville
-- +----+-------------------+------------------------+------+-------+-----+------+--------+
-- | id | origin_airport_id | destination_airport_id | year | month | day | hour | minute |
-- +----+-------------------+------------------------+------+-------+-----+------+--------+
-- | 36 | 8                 | 4                      | 2021 | 7     | 29  | 8    | 20     |
-- +----+-------------------+------------------------+------+-------+-----+------+--------+

-- Checking account numbers with bank records to get person id
-- Match person id with people table and get passport number
-- Match passport number from people table with passport number in passenger table of current flight

SELECT passport_number FROM people WHERE id IN (SELECT person_id FROM bank_accounts WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = 'Leggett Street'));

SELECT passport_number FROM passengers WHERE flight_id = (SELECT id FROM flights WHERE year = 2021 AND month = 7 AND day = 29 ORDER BY hour ASC LIMIT 1);

-- Intersect common passport nums
SELECT DISTINCT(passport_number) FROM passengers WHERE passport_number in (SELECT passport_number FROM people WHERE id IN (SELECT person_id FROM bank_accounts WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = 'Leggett Street'))) AND passport_number in (SELECT passport_number FROM passengers WHERE flight_id = (SELECT id FROM flights WHERE year = 2021 AND month = 7 AND day = 29 ORDER BY hour ASC LIMIT 1));
-- +-----------------+
-- | passport_number |
-- +-----------------+
-- | 8496433585      |
-- | 5773159633      |
-- | 1988161715      |
-- | 9878712108      |
-- +-----------------+

-- These 5 people were at atm and booked the flight next day

-- Taking full details about these passport holders
SELECT * FROM people WHERE passport_number IN (SELECT DISTINCT(passport_number) FROM passengers WHERE passport_number in (SELECT passport_number FROM people WHERE id IN (SELECT person_id FROM bank_accounts WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = 'Leggett Street'))) AND passport_number in (SELECT passport_number FROM passengers WHERE flight_id = (SELECT id FROM flights WHERE year = 2021 AND month = 7 AND day = 29 ORDER BY hour ASC LIMIT 1)));
-- +--------+--------+----------------+-----------------+---------------+
-- |   id   |  name  |  phone_number  | passport_number | license_plate |
-- +--------+--------+----------------+-----------------+---------------+
-- | 395717 | Kenny  | (826) 555-1652 | 9878712108      | 30G67EN       |
-- | 449774 | Taylor | (286) 555-6063 | 1988161715      | 1106N58       |
-- | 467400 | Luca   | (389) 555-5198 | 8496433585      | 4328GD8       |
-- | 686048 | Bruce  | (367) 555-5533 | 5773159633      | 94KL13X       |
-- +--------+--------+----------------+-----------------+---------------+

-- Now we need to match the phone number of these passport holders with our records

SELECT * FROM people WHERE phone_number IN (SELECT caller FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60) AND phone_number IN (SELECT phone_number FROM people WHERE passport_number IN (SELECT DISTINCT(passport_number) FROM passengers WHERE passport_number in (SELECT passport_number FROM people WHERE id IN (SELECT person_id FROM bank_accounts WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = 'Leggett Street'))) AND passport_number in (SELECT passport_number FROM passengers WHERE flight_id = (SELECT id FROM flights WHERE year = 2021 AND month = 7 AND day = 29 ORDER BY hour ASC LIMIT 1))));
-- +--------+--------+----------------+-----------------+---------------+
-- |   id   |  name  |  phone_number  | passport_number | license_plate |
-- +--------+--------+----------------+-----------------+---------------+
-- | 395717 | Kenny  | (826) 555-1652 | 9878712108      | 30G67EN       |
-- | 449774 | Taylor | (286) 555-6063 | 1988161715      | 1106N58       |
-- | 686048 | Bruce  | (367) 555-5533 | 5773159633      | 94KL13X       |
-- +--------+--------+----------------+-----------------+---------------+
-- All of them made call on the specific day and booked flight the next day

-- Now we need to check these license plates with our bakery_security_log
-- SELECT * FROM people WHERE license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE  year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute >= 15 AND minute <= 25 AND activity = 'exit') AND license_plate IN (SELECT license_plate FROM people WHERE phone_number IN (SELECT caller FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60) AND phone_number IN (SELECT phone_number FROM people WHERE passport_number IN (SELECT DISTINCT(passport_number) FROM passengers WHERE passport_number in (SELECT passport_number FROM people WHERE id IN (SELECT person_id FROM bank_accounts WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = 'Leggett Street'))) AND passport_number in (SELECT passport_number FROM passengers WHERE flight_id = (SELECT id FROM flights WHERE year = 2021 AND month = 7 AND day = 29 ORDER BY hour ASC LIMIT 1)))));
-- The above online line query and the query below are the same
SELECT *
FROM people
WHERE license_plate IN (
    SELECT license_plate
    FROM bakery_security_logs
    WHERE  year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute >= 15 AND minute <= 25 AND activity = 'exit'
) AND license_plate IN (
    SELECT license_plate
    FROM people
    WHERE phone_number IN (
        SELECT caller
        FROM phone_calls
        WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60
    ) AND phone_number IN (
        SELECT phone_number
        FROM people
        WHERE passport_number IN (
            SELECT DISTINCT(passport_number)
            FROM passengers
            WHERE passport_number in (
                SELECT passport_number
                FROM people
                WHERE id IN (
                    SELECT person_id
                    FROM bank_accounts
                    WHERE account_number IN (
                        SELECT account_number
                        FROM atm_transactions
                        WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = 'Leggett Street'
                    )
                )
            ) AND passport_number in (
                SELECT passport_number
                FROM passengers
                WHERE flight_id = (
                    SELECT id
                    FROM flights
                    WHERE year = 2021 AND month = 7 AND day = 29 ORDER BY hour ASC LIMIT 1
                )
            )
        )
    )
);

-- +--------+-------+----------------+-----------------+---------------+
-- |   id   | name  |  phone_number  | passport_number | license_plate |
-- +--------+-------+----------------+-----------------+---------------+
-- | 686048 | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       |
-- +--------+-------+----------------+-----------------+---------------+
-- These two people were in bakery, made atm transaction, made calls less than a minute, booked earliest flight out of fiftyville next day

-- So we have narrowed down our suspect list to a single person
-- Now we need to check all activities of bruce to verify the case




----------------------------------------------------------------------------------------------------------------------




-- +--------+-------+----------------+-----------------+---------------+
-- |   id   | name  |  phone_number  | passport_number | license_plate |
-- +--------+-------+----------------+-----------------+---------------+
-- | 686048 | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       |
-- +--------+-------+----------------+-----------------+---------------+

-- When did he left bakery parking lot?
SELECT * FROM bakery_security_logs WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute >= 15 AND minute <= 25 AND license_plate = '94KL13X';
-- He left at 10:18 am

-- Did he used ATM?
SELECT * FROM atm_transactions WHERE account_number = (SELECT account_number FROM bank_accounts WHERE person_id = 686048);
-- He used at Legget Street's ATM to withdraw $50

-- Whom did he called?
SELECT * FROM phone_calls WHERE caller = '(367) 555-5533' AND year = 2021 AND month = 7 AND day = 28 AND duration < 60;
-- +-----+----------------+----------------+------+-------+-----+----------+
-- | id  |     caller     |    receiver    | year | month | day | duration |
-- +-----+----------------+----------------+------+-------+-----+----------+
-- | 233 | (367) 555-5533 | (375) 555-8161 | 2021 | 7     | 28  | 45       |
-- +-----+----------------+----------------+------+-------+-----+----------+

-- Whose phone number is (375) 555-8161?
SELECT * FROM people WHERE phone_number = '(375) 555-8161';
-- +--------+-------+----------------+-----------------+---------------+
-- |   id   | name  |  phone_number  | passport_number | license_plate |
-- +--------+-------+----------------+-----------------+---------------+
-- | 864400 | Robin | (375) 555-8161 | NULL            | 4V16VO0       |
-- +--------+-------+----------------+-----------------+---------------+

-- Where did he escaped to?
SELECT city FROM airports WHERE id = (SELECT destination_airport_id FROM flights WHERE year = 2021 AND month = 7 AND day = 29 ORDER BY hour ASC LIMIT 1);
-- +---------------+
-- |     city      |
-- +---------------+
-- | New York City |
-- +---------------+