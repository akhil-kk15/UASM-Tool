-- Seed Data for UASM-Tool (Utilities and Schedule Management Tool)
-- 15 Teams with Table Game Assignments (Team Alpha & Team Bravo dedicated to Blackjack)
-- Shift Limit: Up to 7 employees per team in an 8-hour shift (target_staff = 7, min_staff = 5, max_staff = 7)
-- 105 Realistic Employees with 5-Digit IDs (10001 - 10105) and Game Knowledge (>68% knowing 3-6 games)
-- Seeded with overstaffed (8-10/7) and understaffed (4-5/7) teams so clicking "Auto-Balance Staff"
-- immediately demonstrates ~12 game-knowledge-matched reassignments in the Recent Changes sidebar.

PRAGMA foreign_keys = OFF;

DROP TABLE IF EXISTS uniform_issues;
DROP TABLE IF EXISTS reassignments;
DROP TABLE IF EXISTS uniforms;
DROP TABLE IF EXISTS employees;
DROP TABLE IF EXISTS teams;

CREATE TABLE teams (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL UNIQUE,
    team_type TEXT DEFAULT 'Dealer',
    uniform_type TEXT NOT NULL,
    target_staff INTEGER DEFAULT 7,
    min_staff INTEGER DEFAULT 5,
    max_staff INTEGER DEFAULT 7,
    table_games TEXT DEFAULT 'Blackjack, Blackjack, Blackjack, Blackjack',
    description TEXT
);

CREATE TABLE employees (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    role_type TEXT DEFAULT 'Dealer',
    department TEXT,
    designation TEXT,
    contact_number TEXT,
    email TEXT,
    uniform_size TEXT,
    game_knowledge TEXT DEFAULT 'Blackjack, Roulette',
    join_date TEXT DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE uniforms (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    type TEXT NOT NULL,
    team TEXT DEFAULT '',
    size TEXT NOT NULL,
    quantity INTEGER NOT NULL DEFAULT 0,
    price REAL NOT NULL DEFAULT 0.0,
    color TEXT,
    description TEXT,
    created_date TEXT DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE uniform_issues (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    employee_id INTEGER NOT NULL,
    uniform_id INTEGER NOT NULL,
    quantity INTEGER NOT NULL,
    issue_date TEXT NOT NULL,
    return_date TEXT,
    status TEXT DEFAULT 'Issued',
    notes TEXT,
    FOREIGN KEY(employee_id) REFERENCES employees(id),
    FOREIGN KEY(uniform_id) REFERENCES uniforms(id)
);

CREATE TABLE reassignments (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    employee_id INTEGER NOT NULL,
    from_team TEXT,
    to_team TEXT NOT NULL,
    uniform_id INTEGER,
    reassignment_date TEXT DEFAULT CURRENT_TIMESTAMP,
    notes TEXT
);

PRAGMA foreign_keys = ON;

-- ============================================================================
-- 1. TEAMS (15 Teams, 4 Game Tables per Team, Up to 7 Staff per 8-Hour Shift)
-- Team Alpha and Team Bravo are dedicated strictly to Blackjack tables.
-- ============================================================================
INSERT INTO teams (id, name, team_type, uniform_type, target_staff, min_staff, max_staff, table_games, description) VALUES
(1,  'Team Alpha',    'Dealer',         'Team Alpha Vest',    7, 5, 7, 'Blackjack, Blackjack, Blackjack, Blackjack', 'Dedicated Blackjack pit (Tables 1-4 Blackjack)'),
(2,  'Team Bravo',    'Dealer',         'Team Bravo Vest',    7, 5, 7, 'Blackjack, Blackjack, Blackjack, Blackjack', 'Dedicated Blackjack pit (Tables 5-8 Blackjack)'),
(3,  'Team Charlie',  'Dealer',         'Team Charlie Vest',  7, 5, 7, 'Blackjack, Blackjack, Roulette, Roulette',   'Main floor Blackjack and Roulette tables'),
(4,  'Team Delta',    'Dealer',         'Team Delta Vest',    7, 5, 7, 'Blackjack, Roulette, Blackjack, Roulette',   'South floor Blackjack and Roulette tables'),
(5,  'Team Echo',     'Dealer',         'Team Echo Vest',     7, 5, 7, 'Blackjack, Roulette, Baccarat, Baccarat',    'Asian games & main floor pit'),
(6,  'Team Foxtrot',  'Dealer',         'Team Foxtrot Vest',  7, 5, 7, 'Baccarat, Baccarat, Roulette, Blackjack',    'High-limit Baccarat, Roulette & Blackjack'),
(7,  'Team Golf',     'Dealer',         'Team Golf Vest',     7, 5, 7, 'Poker, Poker, Blackjack, Roulette',          'Casino Poker, Blackjack & Roulette pit'),
(8,  'Team Hotel',    'Dealer',         'Team Hotel Vest',    7, 5, 7, 'Blackjack, Baccarat, Poker, Roulette',       'Mixed 4-game international pit'),
(9,  'Team India',    'Dealer',         'Team India Vest',    7, 5, 7, 'Craps, Craps, Blackjack, Roulette',          'Dice & wheel pit (Craps, Blackjack, Roulette)'),
(10, 'Team Juliet',   'Dealer',         'Team Juliet Vest',   7, 5, 7, 'Baccarat, Poker, Craps, Blackjack',          'VIP multi-game salon'),
(11, 'Team Kilo',     'Dealer',         'Team Kilo Vest',     7, 5, 7, 'Roulette, Roulette, Baccarat, Poker',        'European wheel & card salon'),
(12, 'Team Lima',     'Dealer',         'Team Lima Vest',     7, 5, 7, 'Poker, Craps, Baccarat, Roulette',           'Premier specialty table games pit'),
(13, 'Team Mike',     'Dealer',         'Team Mike Vest',     7, 5, 7, 'Blackjack, Roulette, Poker, Craps',          'Grand floor multi-game rotation'),
(14, 'Studio Live',   'Game Show Host', 'Studio Live Suit',   7, 5, 7, 'Game Show, Game Show, Game Show, Game Show', 'Live broadcast Game Show Studio A'),
(15, 'Studio Royale', 'Game Show Host', 'Studio Royale Suit', 7, 5, 7, 'Game Show, Game Show, Game Show, Game Show', 'VIP broadcast Game Show Studio B');

-- ============================================================================
-- 2. UNIFORMS (60 records: S, M, L, XL for all 15 Teams with ample stock)
-- ============================================================================
INSERT INTO uniforms (id, type, team, size, quantity, price, color, description) VALUES
-- Team Alpha (1-4)
(1,  'Team Alpha Vest',    'Team Alpha',    'S',  10, 45.00, 'Crimson Red',   'Team Alpha Blackjack Pit Vest (S)'),
(2,  'Team Alpha Vest',    'Team Alpha',    'M',  12, 45.00, 'Crimson Red',   'Team Alpha Blackjack Pit Vest (M)'),
(3,  'Team Alpha Vest',    'Team Alpha',    'L',  12, 45.00, 'Crimson Red',   'Team Alpha Blackjack Pit Vest (L)'),
(4,  'Team Alpha Vest',    'Team Alpha',    'XL', 8,  48.00, 'Crimson Red',   'Team Alpha Blackjack Pit Vest (XL)'),
-- Team Bravo (5-8)
(5,  'Team Bravo Vest',    'Team Bravo',    'S',  10, 45.00, 'Royal Blue',    'Team Bravo Blackjack Pit Vest (S)'),
(6,  'Team Bravo Vest',    'Team Bravo',    'M',  12, 45.00, 'Royal Blue',    'Team Bravo Blackjack Pit Vest (M)'),
(7,  'Team Bravo Vest',    'Team Bravo',    'L',  12, 45.00, 'Royal Blue',    'Team Bravo Blackjack Pit Vest (L)'),
(8,  'Team Bravo Vest',    'Team Bravo',    'XL', 8,  48.00, 'Royal Blue',    'Team Bravo Blackjack Pit Vest (XL)'),
-- Team Charlie (9-12)
(9,  'Team Charlie Vest',  'Team Charlie',  'S',  10, 45.00, 'Emerald Green', 'Team Charlie Dealer Vest (S)'),
(10, 'Team Charlie Vest',  'Team Charlie',  'M',  12, 45.00, 'Emerald Green', 'Team Charlie Dealer Vest (M)'),
(11, 'Team Charlie Vest',  'Team Charlie',  'L',  12, 45.00, 'Emerald Green', 'Team Charlie Dealer Vest (L)'),
(12, 'Team Charlie Vest',  'Team Charlie',  'XL', 8,  48.00, 'Emerald Green', 'Team Charlie Dealer Vest (XL)'),
-- Team Delta (13-16)
(13, 'Team Delta Vest',    'Team Delta',    'S',  10, 45.00, 'Deep Purple',   'Team Delta Dealer Vest (S)'),
(14, 'Team Delta Vest',    'Team Delta',    'M',  12, 45.00, 'Deep Purple',   'Team Delta Dealer Vest (M)'),
(15, 'Team Delta Vest',    'Team Delta',    'L',  12, 45.00, 'Deep Purple',   'Team Delta Dealer Vest (L)'),
(16, 'Team Delta Vest',    'Team Delta',    'XL', 8,  48.00, 'Deep Purple',   'Team Delta Dealer Vest (XL)'),
-- Team Echo (17-20)
(17, 'Team Echo Vest',     'Team Echo',     'S',  10, 46.00, 'Charcoal Grey', 'Team Echo Dealer Vest (S)'),
(18, 'Team Echo Vest',     'Team Echo',     'M',  12, 46.00, 'Charcoal Grey', 'Team Echo Dealer Vest (M)'),
(19, 'Team Echo Vest',     'Team Echo',     'L',  12, 46.00, 'Charcoal Grey', 'Team Echo Dealer Vest (L)'),
(20, 'Team Echo Vest',     'Team Echo',     'XL', 8,  49.00, 'Charcoal Grey', 'Team Echo Dealer Vest (XL)'),
-- Team Foxtrot (21-24)
(21, 'Team Foxtrot Vest',  'Team Foxtrot',  'S',  10, 48.00, 'Burgundy Gold', 'Team Foxtrot High-Limit Vest (S)'),
(22, 'Team Foxtrot Vest',  'Team Foxtrot',  'M',  12, 48.00, 'Burgundy Gold', 'Team Foxtrot High-Limit Vest (M)'),
(23, 'Team Foxtrot Vest',  'Team Foxtrot',  'L',  12, 48.00, 'Burgundy Gold', 'Team Foxtrot High-Limit Vest (L)'),
(24, 'Team Foxtrot Vest',  'Team Foxtrot',  'XL', 8,  50.00, 'Burgundy Gold', 'Team Foxtrot High-Limit Vest (XL)'),
-- Team Golf (25-28)
(25, 'Team Golf Vest',     'Team Golf',     'S',  10, 46.00, 'Forest Green',  'Team Golf Poker & Table Vest (S)'),
(26, 'Team Golf Vest',     'Team Golf',     'M',  12, 46.00, 'Forest Green',  'Team Golf Poker & Table Vest (M)'),
(27, 'Team Golf Vest',     'Team Golf',     'L',  12, 46.00, 'Forest Green',  'Team Golf Poker & Table Vest (L)'),
(28, 'Team Golf Vest',     'Team Golf',     'XL', 8,  49.00, 'Forest Green',  'Team Golf Poker & Table Vest (XL)'),
-- Team Hotel (29-32)
(29, 'Team Hotel Vest',    'Team Hotel',    'S',  10, 48.00, 'Navy Silver',   'Team Hotel International Pit Vest (S)'),
(30, 'Team Hotel Vest',    'Team Hotel',    'M',  12, 48.00, 'Navy Silver',   'Team Hotel International Pit Vest (M)'),
(31, 'Team Hotel Vest',    'Team Hotel',    'L',  12, 48.00, 'Navy Silver',   'Team Hotel International Pit Vest (L)'),
(32, 'Team Hotel Vest',    'Team Hotel',    'XL', 8,  50.00, 'Navy Silver',   'Team Hotel International Pit Vest (XL)'),
-- Team India (33-36)
(33, 'Team India Vest',    'Team India',    'S',  10, 46.00, 'Copper Bronze', 'Team India Dice & Wheel Vest (S)'),
(34, 'Team India Vest',    'Team India',    'M',  12, 46.00, 'Copper Bronze', 'Team India Dice & Wheel Vest (M)'),
(35, 'Team India Vest',    'Team India',    'L',  12, 46.00, 'Copper Bronze', 'Team India Dice & Wheel Vest (L)'),
(36, 'Team India Vest',    'Team India',    'XL', 8,  49.00, 'Copper Bronze', 'Team India Dice & Wheel Vest (XL)'),
-- Team Juliet (37-40)
(37, 'Team Juliet Vest',   'Team Juliet',   'S',  10, 52.00, 'Onyx Black',    'Team Juliet VIP Salon Vest (S)'),
(38, 'Team Juliet Vest',   'Team Juliet',   'M',  12, 52.00, 'Onyx Black',    'Team Juliet VIP Salon Vest (M)'),
(39, 'Team Juliet Vest',   'Team Juliet',   'L',  12, 52.00, 'Onyx Black',    'Team Juliet VIP Salon Vest (L)'),
(40, 'Team Juliet Vest',   'Team Juliet',   'XL', 8,  55.00, 'Onyx Black',    'Team Juliet VIP Salon Vest (XL)'),
-- Team Kilo (41-44)
(41, 'Team Kilo Vest',     'Team Kilo',     'S',  10, 48.00, 'Teal Sapphire', 'Team Kilo European Salon Vest (S)'),
(42, 'Team Kilo Vest',     'Team Kilo',     'M',  12, 48.00, 'Teal Sapphire', 'Team Kilo European Salon Vest (M)'),
(43, 'Team Kilo Vest',     'Team Kilo',     'L',  12, 48.00, 'Teal Sapphire', 'Team Kilo European Salon Vest (L)'),
(44, 'Team Kilo Vest',     'Team Kilo',     'XL', 8,  50.00, 'Teal Sapphire', 'Team Kilo European Salon Vest (XL)'),
-- Team Lima (45-48)
(45, 'Team Lima Vest',     'Team Lima',     'S',  10, 50.00, 'Plum Velvet',   'Team Lima Specialty Pit Vest (S)'),
(46, 'Team Lima Vest',     'Team Lima',     'M',  12, 50.00, 'Plum Velvet',   'Team Lima Specialty Pit Vest (M)'),
(47, 'Team Lima Vest',     'Team Lima',     'L',  12, 50.00, 'Plum Velvet',   'Team Lima Specialty Pit Vest (L)'),
(48, 'Team Lima Vest',     'Team Lima',     'XL', 8,  53.00, 'Plum Velvet',   'Team Lima Specialty Pit Vest (XL)'),
-- Team Mike (49-52)
(49, 'Team Mike Vest',     'Team Mike',     'S',  10, 48.00, 'Slate Blue',    'Team Mike Grand Floor Vest (S)'),
(50, 'Team Mike Vest',     'Team Mike',     'M',  12, 48.00, 'Slate Blue',    'Team Mike Grand Floor Vest (M)'),
(51, 'Team Mike Vest',     'Team Mike',     'L',  12, 48.00, 'Slate Blue',    'Team Mike Grand Floor Vest (L)'),
(52, 'Team Mike Vest',     'Team Mike',     'XL', 8,  50.00, 'Slate Blue',    'Team Mike Grand Floor Vest (XL)'),
-- Studio Live (53-56)
(53, 'Studio Live Suit',   'Studio Live',   'S',  10, 75.00, 'Gold Trim',     'Studio Live Game Show Host Suit (S)'),
(54, 'Studio Live Suit',   'Studio Live',   'M',  12, 75.00, 'Gold Trim',     'Studio Live Game Show Host Suit (M)'),
(55, 'Studio Live Suit',   'Studio Live',   'L',  12, 75.00, 'Gold Trim',     'Studio Live Game Show Host Suit (L)'),
(56, 'Studio Live Suit',   'Studio Live',   'XL', 8,  78.00, 'Gold Trim',     'Studio Live Game Show Host Suit (XL)'),
-- Studio Royale (57-60)
(57, 'Studio Royale Suit', 'Studio Royale', 'S',  10, 80.00, 'Silver Trim',   'Studio Royale VIP Host Suit (S)'),
(58, 'Studio Royale Suit', 'Studio Royale', 'M',  12, 80.00, 'Silver Trim',   'Studio Royale VIP Host Suit (M)'),
(59, 'Studio Royale Suit', 'Studio Royale', 'L',  12, 80.00, 'Silver Trim',   'Studio Royale VIP Host Suit (L)'),
(60, 'Studio Royale Suit', 'Studio Royale', 'XL', 8,  84.00, 'Silver Trim',   'Studio Royale VIP Host Suit (XL)');

-- ============================================================================
-- 3. EMPLOYEES (105 Realistic Casino Employees with 5-Digit IDs: 10001 - 10105)
-- 70/30 Game Knowledge Distribution:
--   - 70% Multi-Game / All-Game Knowledge: 74 / 105 Employees (70.5%) know all/multiple table games
--   - 30% Basic Game Knowledge:            31 / 105 Employees (29.5%) know Basic games
--       (23 Basic Dealers with 'Blackjack, Roulette' + 8 Basic Hosts with 'Game Show')
-- All 13 overflow employees across overstaffed teams possess full multi-game knowledge so that
-- clicking "Auto-Balance Staff" balances all 15 teams to 7/7 Optimal (30-Min shift rotation).
-- ============================================================================

INSERT INTO employees (id, name, role_type, department, designation, contact_number, email, uniform_size, game_knowledge, join_date) VALUES
-- Team Alpha (10 members - Overstaffed 10/7; 6 Basic BJ/Roulette + 4 Full Multi-Game; 3 overflow relocate on Auto-Balance)
(10001, 'Alexander Vance',     'Dealer', 'Team Alpha', 'Senior Pit Dealer',  '555-2001', 'alexander.vance@uasm.org',     'L',  'Blackjack, Roulette, Baccarat, Poker, Craps',            '2023-01-12'),
(10002, 'Victoria Sterling',   'Dealer', 'Team Alpha', 'Blackjack Dealer',   '555-2002', 'victoria.sterling@uasm.org',   'M',  'Blackjack, Roulette',                                    '2023-02-04'),
(10003, 'Benjamin Hayes',      'Dealer', 'Team Alpha', 'Blackjack Dealer',   '555-2003', 'benjamin.hayes@uasm.org',      'L',  'Blackjack, Roulette',                                    '2023-03-18'),
(10004, 'Christopher Morgan',  'Dealer', 'Team Alpha', 'Blackjack Dealer',   '555-2004', 'christopher.morgan@uasm.org',  'M',  'Blackjack, Roulette',                                    '2023-05-21'),
(10005, 'Samantha Price',      'Dealer', 'Team Alpha', 'Blackjack Dealer',   '555-2005', 'samantha.price@uasm.org',      'S',  'Blackjack, Roulette',                                    '2023-06-14'),
(10006, 'Daniel Foster',       'Dealer', 'Team Alpha', 'Blackjack Dealer',   '555-2006', 'daniel.foster@uasm.org',       'XL', 'Blackjack, Roulette',                                    '2023-07-02'),
(10007, 'Caroline Bennett',    'Dealer', 'Team Alpha', 'Blackjack Dealer',   '555-2007', 'caroline.bennett@uasm.org',    'M',  'Blackjack, Roulette',                                    '2023-08-19'),
(10008, 'Matthew Coleman',     'Both',   'Team Alpha', 'Dual Floor Dealer',  '555-2008', 'matthew.coleman@uasm.org',     'L',  'Blackjack, Roulette, Baccarat, Poker, Craps, Game Show', '2023-09-05'),
(10009, 'Natalie Brooks',      'Dealer', 'Team Alpha', 'Multi-Game Dealer',  '555-2009', 'natalie.brooks@uasm.org',      'S',  'Blackjack, Roulette, Baccarat, Poker, Craps',            '2023-09-12'),
(10010, 'Harrison Reed',       'Dealer', 'Team Alpha', 'Multi-Game Dealer',  '555-2010', 'harrison.reed@uasm.org',       'M',  'Blackjack, Roulette, Baccarat, Poker, Craps',            '2023-09-18'),

-- Team Bravo (9 members - Overstaffed 9/7; 6 Basic BJ/Roulette + 3 Full Multi-Game; 2 overflow relocate on Auto-Balance)
(10011, 'Nicholas Perry',      'Dealer', 'Team Bravo', 'Senior Pit Dealer',  '555-2011', 'nicholas.perry@uasm.org',      'L',  'Blackjack, Roulette, Baccarat, Poker, Craps',            '2023-01-20'),
(10012, 'Lauren Henderson',    'Dealer', 'Team Bravo', 'Blackjack Dealer',   '555-2012', 'lauren.henderson@uasm.org',    'M',  'Blackjack, Roulette',                                    '2023-02-15'),
(10013, 'Jonathan Ross',       'Dealer', 'Team Bravo', 'Blackjack Dealer',   '555-2013', 'jonathan.ross@uasm.org',       'M',  'Blackjack, Roulette',                                    '2023-03-22'),
(10014, 'Megan Butler',        'Dealer', 'Team Bravo', 'Blackjack Dealer',   '555-2014', 'megan.butler@uasm.org',        'S',  'Blackjack, Roulette',                                    '2023-04-17'),
(10015, 'Anthony Simmons',     'Dealer', 'Team Bravo', 'Blackjack Dealer',   '555-2015', 'anthony.simmons@uasm.org',     'XL', 'Blackjack, Roulette',                                    '2023-05-29'),
(10016, 'Hannah Flores',       'Dealer', 'Team Bravo', 'Blackjack Dealer',   '555-2016', 'hannah.flores@uasm.org',       'S',  'Blackjack, Roulette',                                    '2023-06-30'),
(10017, 'Kevin Washington',    'Dealer', 'Team Bravo', 'Blackjack Dealer',   '555-2017', 'kevin.washington@uasm.org',    'L',  'Blackjack, Roulette',                                    '2023-07-14'),
(10018, 'Rachel Simmons',      'Dealer', 'Team Bravo', 'Multi-Game Dealer',  '555-2018', 'rachel.simmons@uasm.org',      'M',  'Blackjack, Roulette, Baccarat, Poker, Craps',            '2023-08-25'),
(10019, 'Marcus Vance',        'Both',   'Team Bravo', 'Dual Floor Host',    '555-2019', 'marcus.vance@uasm.org',        'L',  'Blackjack, Roulette, Baccarat, Poker, Craps, Game Show', '2023-09-02'),

-- Team Charlie (9 members - Overstaffed 9/7; 6 Basic BJ/Roulette + 3 Full Multi-Game; 2 overflow relocate on Auto-Balance)
(10020, 'Brandon Patterson',   'Dealer', 'Team Charlie', 'Senior Pit Dealer','555-2020', 'brandon.patterson@uasm.org',   'L',  'Blackjack, Roulette, Baccarat, Poker, Craps',            '2023-01-25'),
(10021, 'Olivia Hughes',       'Dealer', 'Team Charlie', 'Table Dealer',     '555-2021', 'olivia.hughes@uasm.org',       'M',  'Blackjack, Roulette',                                    '2023-02-18'),
(10022, 'Christian Gonzales',  'Dealer', 'Team Charlie', 'Table Dealer',     '555-2022', 'christian.gonzales@uasm.org',  'M',  'Blackjack, Roulette',                                    '2023-03-27'),
(10023, 'Grace Alexander',     'Dealer', 'Team Charlie', 'Table Dealer',     '555-2023', 'grace.alexander@uasm.org',     'S',  'Blackjack, Roulette',                                    '2023-04-12'),
(10024, 'Samuel Russell',      'Dealer', 'Team Charlie', 'Table Dealer',     '555-2024', 'samuel.russell@uasm.org',      'XL', 'Blackjack, Roulette',                                    '2023-05-08'),
(10025, 'Chloe Griffin',       'Dealer', 'Team Charlie', 'Table Dealer',     '555-2025', 'chloe.griffin@uasm.org',       'S',  'Blackjack, Roulette',                                    '2023-06-19'),
(10026, 'Gregory Diaz',        'Dealer', 'Team Charlie', 'Table Dealer',     '555-2026', 'gregory.diaz@uasm.org',        'L',  'Blackjack, Roulette',                                    '2023-07-23'),
(10027, 'Zoe Hayes',           'Dealer', 'Team Charlie', 'Multi-Game Dealer','555-2027', 'zoe.hayes@uasm.org',           'M',  'Blackjack, Roulette, Baccarat, Poker, Craps',            '2023-08-30'),
(10028, 'Vincent Mercer',      'Dealer', 'Team Charlie', 'Multi-Game Dealer','555-2028', 'vincent.mercer@uasm.org',      'L',  'Blackjack, Roulette, Baccarat, Poker, Craps',            '2023-09-14'),

-- Team Delta (5 members - Understaffed 5/7 -> 40-Min Rotation; 5 Basic BJ/Roulette)
(10029, 'Patrick Myers',       'Dealer', 'Team Delta', 'Senior Pit Dealer',  '555-2029', 'patrick.myers@uasm.org',       'L',  'Blackjack, Roulette',                                    '2023-01-30'),
(10030, 'Audrey Ford',         'Dealer', 'Team Delta', 'Table Dealer',       '555-2030', 'audrey.ford@uasm.org',         'M',  'Blackjack, Roulette',                                    '2023-02-22'),
(10031, 'Julian Hamilton',     'Dealer', 'Team Delta', 'Table Dealer',       '555-2031', 'julian.hamilton@uasm.org',     'M',  'Blackjack, Roulette',                                    '2023-03-15'),
(10032, 'Claire Graham',       'Dealer', 'Team Delta', 'Table Dealer',       '555-2032', 'claire.graham@uasm.org',       'S',  'Blackjack, Roulette',                                    '2023-04-25'),
(10033, 'Aaron Sullivan',      'Dealer', 'Team Delta', 'Table Dealer',       '555-2033', 'aaron.sullivan@uasm.org',      'XL', 'Blackjack, Roulette',                                    '2023-05-17'),

-- Team Echo (4 members - Critically Understaffed 4/7 -> Dire 20-Min Rotation; Blackjack, Roulette, Baccarat)
(10034, 'Thomas West',         'Dealer', 'Team Echo', 'Senior Pit Dealer',   '555-2034', 'thomas.west@uasm.org',         'L',  'Blackjack, Roulette, Baccarat, Poker, Craps',            '2023-01-14'),
(10035, 'Eleanor Jordan',      'Dealer', 'Team Echo', 'Senior Dealer',       '555-2035', 'eleanor.jordan@uasm.org',      'M',  'Blackjack, Roulette, Baccarat, Poker, Craps',            '2023-02-09'),
(10036, 'Sebastian Owens',     'Dealer', 'Team Echo', 'Baccarat Dealer',     '555-2036', 'sebastian.owens@uasm.org',     'M',  'Blackjack, Roulette, Baccarat, Poker',                   '2023-03-21'),
(10037, 'Penelope Reynolds',   'Dealer', 'Team Echo', 'Baccarat Dealer',     '555-2037', 'penelope.reynolds@uasm.org',   'S',  'Blackjack, Roulette, Baccarat',                          '2023-04-18'),

-- Team Foxtrot (8 members - Overstaffed 8/7; 1 overflow member ready to relocate on Auto-Balance)
(10038, 'Owen Harrison',       'Dealer', 'Team Foxtrot', 'Senior Pit Dealer','555-2038', 'owen.harrison@uasm.org',       'L',  'Blackjack, Roulette, Baccarat, Poker, Craps',            '2023-01-19'),
(10039, 'Hazel Gibson',        'Dealer', 'Team Foxtrot', 'Senior Dealer',    '555-2039', 'hazel.gibson@uasm.org',        'S',  'Blackjack, Roulette, Baccarat, Poker, Craps',            '2023-02-24'),
(10040, 'Gabriel McDonald',    'Dealer', 'Team Foxtrot', 'VIP Dealer',       '555-2040', 'gabriel.mcdonald@uasm.org',    'M',  'Blackjack, Roulette, Baccarat, Craps',                   '2023-03-29'),
(10041, 'Violet Cruz',         'Dealer', 'Team Foxtrot', 'VIP Dealer',       '555-2041', 'violet.cruz@uasm.org',         'M',  'Blackjack, Roulette, Baccarat, Poker',                   '2023-04-14'),
(10042, 'Carter Marshall',     'Dealer', 'Team Foxtrot', 'Baccarat Dealer',  '555-2042', 'carter.marshall@uasm.org',     'XL', 'Blackjack, Roulette, Baccarat',                          '2023-05-19'),
(10043, 'Aurora Ortiz',        'Dealer', 'Team Foxtrot', 'Baccarat Dealer',  '555-2043', 'aurora.ortiz@uasm.org',        'S',  'Blackjack, Roulette, Baccarat',                          '2023-06-22'),
(10044, 'Wyatt Gomez',         'Dealer', 'Team Foxtrot', 'Baccarat Dealer',  '555-2044', 'wyatt.gomez@uasm.org',         'L',  'Blackjack, Roulette, Baccarat',                          '2023-07-28'),
(10045, 'Henry Fisher',        'Dealer', 'Team Foxtrot', 'Multi-Game Dealer','555-2045', 'henry.fisher@uasm.org',        'XL', 'Blackjack, Roulette, Baccarat, Poker, Craps',            '2023-08-15'),

-- Team Golf (9 members - Overstaffed 9/7; 2 overflow members ready to relocate on Auto-Balance)
(10046, 'Luke Murray',         'Dealer', 'Team Golf', 'Senior Pit Dealer',   '555-2046', 'luke.murray@uasm.org',         'L',  'Blackjack, Roulette, Baccarat, Poker, Craps',            '2023-01-22'),
(10047, 'Savannah Freeman',    'Dealer', 'Team Golf', 'Senior Dealer',       '555-2047', 'savannah.freeman@uasm.org',    'M',  'Blackjack, Roulette, Baccarat, Poker, Craps',            '2023-02-11'),
(10048, 'Jayden Wells',        'Dealer', 'Team Golf', 'Poker Specialist',    '555-2048', 'jayden.wells@uasm.org',        'M',  'Blackjack, Roulette, Baccarat, Poker',                   '2023-03-16'),
(10049, 'Bella Webb',          'Dealer', 'Team Golf', 'Poker Specialist',    '555-2049', 'bella.webb@uasm.org',          'S',  'Blackjack, Roulette, Poker, Craps',                      '2023-04-21'),
(10050, 'Dylan Simpson',       'Dealer', 'Team Golf', 'Poker Dealer',        '555-2050', 'dylan.simpson@uasm.org',       'XL', 'Blackjack, Roulette, Poker',                             '2023-05-12'),
(10051, 'Nora Stevens',        'Dealer', 'Team Golf', 'Poker Dealer',        '555-2051', 'nora.stevens@uasm.org',        'S',  'Blackjack, Roulette, Poker',                             '2023-06-25'),
(10052, 'Grayson Tucker',      'Dealer', 'Team Golf', 'Poker Dealer',        '555-2052', 'grayson.tucker@uasm.org',      'L',  'Blackjack, Roulette, Poker',                             '2023-07-30'),
(10053, 'Stella Ellis',        'Dealer', 'Team Golf', 'Multi-Game Dealer',   '555-2053', 'stella.ellis@uasm.org',        'M',  'Blackjack, Roulette, Baccarat, Poker, Craps',            '2023-08-19'),
(10054, 'Eva Wallace',         'Dealer', 'Team Golf', 'Multi-Game Dealer',   '555-2054', 'eva.wallace@uasm.org',         'S',  'Blackjack, Roulette, Baccarat, Poker, Craps',            '2023-09-04'),

-- Team Hotel (5 members - Understaffed 5/7 -> 40-Min Rotation; Blackjack, Baccarat, Poker, Roulette)
(10055, 'Levi Porter',         'Dealer', 'Team Hotel', 'Senior Pit Dealer',  '555-2055', 'levi.porter@uasm.org',         'M',  'Blackjack, Roulette, Baccarat, Poker, Craps',            '2023-01-16'),
(10056, 'Skylar Hunter',       'Dealer', 'Team Hotel', 'Senior Dealer',      '555-2056', 'skylar.hunter@uasm.org',       'S',  'Blackjack, Roulette, Baccarat, Poker, Craps',            '2023-02-20'),
(10057, 'Isaac Hicks',         'Dealer', 'Team Hotel', 'Multi-Game Dealer',  '555-2057', 'isaac.hicks@uasm.org',         'L',  'Blackjack, Roulette, Baccarat, Poker, Craps',            '2023-03-24'),
(10058, 'Lucy Crawford',       'Dealer', 'Team Hotel', 'Multi-Game Dealer',  '555-2058', 'lucy.crawford@uasm.org',       'M',  'Blackjack, Roulette, Baccarat, Poker',                   '2023-04-28'),
(10059, 'Mateo Henry',         'Dealer', 'Team Hotel', 'Multi-Game Dealer',  '555-2059', 'mateo.henry@uasm.org',         'XL', 'Blackjack, Roulette, Baccarat, Poker',                   '2023-05-14'),

-- Team India (5 members - Understaffed 5/7 -> 40-Min Rotation; Craps, Blackjack, Roulette)
(10060, 'Theodore Morales',    'Dealer', 'Team India', 'Senior Pit Dealer',  '555-2060', 'theodore.morales@uasm.org',    'L',  'Blackjack, Roulette, Baccarat, Poker, Craps',            '2023-01-18'),
(10061, 'Ellie Kennedy',       'Dealer', 'Team India', 'Senior Dealer',      '555-2061', 'ellie.kennedy@uasm.org',       'M',  'Blackjack, Roulette, Baccarat, Poker, Craps',            '2023-02-26'),
(10062, 'Aiden Warren',        'Dealer', 'Team India', 'Dice Specialist',    '555-2062', 'aiden.warren@uasm.org',        'M',  'Blackjack, Roulette, Baccarat, Craps',                   '2023-03-19'),
(10063, 'Emilia Dixon',        'Dealer', 'Team India', 'Dice Specialist',    '555-2063', 'emilia.dixon@uasm.org',        'S',  'Blackjack, Roulette, Poker, Craps',                      '2023-04-23'),
(10064, 'Lincoln Ramos',       'Dealer', 'Team India', 'Craps Dealer',       '555-2064', 'lincoln.ramos@uasm.org',       'XL', 'Blackjack, Roulette, Craps',                             '2023-05-30'),

-- Team Juliet (8 members - Overstaffed 8/7; 1 overflow member ready to relocate on Auto-Balance)
(10065, 'Roman Gordon',        'Dealer', 'Team Juliet', 'VIP Pit Boss',      '555-2065', 'roman.gordon@uasm.org',        'L',  'Blackjack, Roulette, Baccarat, Poker, Craps',            '2023-01-10'),
(10066, 'Madelyn Shaw',        'Dealer', 'Team Juliet', 'VIP Senior Dealer', '555-2066', 'madelyn.shaw@uasm.org',        'M',  'Blackjack, Roulette, Baccarat, Poker, Craps',            '2023-02-14'),
(10067, 'Evan Holmes',         'Dealer', 'Team Juliet', 'VIP Salon Dealer',  '555-2067', 'evan.holmes@uasm.org',         'M',  'Blackjack, Roulette, Baccarat, Poker, Craps',            '2023-03-12'),
(10068, 'Leah Rice',           'Dealer', 'Team Juliet', 'VIP Salon Dealer',  '555-2068', 'leah.rice@uasm.org',           'S',  'Blackjack, Roulette, Baccarat, Poker, Craps',            '2023-04-16'),
(10069, 'Miles Robertson',     'Dealer', 'Team Juliet', 'VIP Salon Dealer',  '555-2069', 'miles.robertson@uasm.org',     'XL', 'Blackjack, Baccarat, Poker, Craps',                      '2023-05-20'),
(10070, 'Addison Hunt',        'Dealer', 'Team Juliet', 'VIP Salon Dealer',  '555-2070', 'addison.hunt@uasm.org',        'S',  'Blackjack, Baccarat, Poker, Craps',                      '2023-06-24'),
(10071, 'Paisley Boyd',        'Dealer', 'Team Juliet', 'Multi-Game Dealer', '555-2071', 'paisley.boyd@uasm.org',        'S',  'Blackjack, Roulette, Baccarat, Poker, Craps',            '2023-06-29'),
(10072, 'Wesley Black',        'Dealer', 'Team Juliet', 'VIP Multi-Game',    '555-2072', 'wesley.black@uasm.org',        'L',  'Blackjack, Roulette, Baccarat, Poker, Craps',            '2023-07-29'),

-- Team Kilo (5 members - Understaffed 5/7 -> 40-Min Rotation; Roulette, Baccarat, Poker)
(10073, 'Dominic Daniels',     'Dealer', 'Team Kilo', 'Senior Pit Dealer',   '555-2073', 'dominic.daniels@uasm.org',     'L',  'Blackjack, Roulette, Baccarat, Poker, Craps',            '2023-01-24'),
(10074, 'Willow Palmer',       'Dealer', 'Team Kilo', 'Senior Dealer',       '555-2074', 'willow.palmer@uasm.org',       'M',  'Blackjack, Roulette, Baccarat, Poker, Craps',            '2023-02-28'),
(10075, 'Austin Mills',        'Dealer', 'Team Kilo', 'Salon Dealer',        '555-2075', 'austin.mills@uasm.org',        'M',  'Blackjack, Roulette, Baccarat, Poker',                   '2023-03-30'),
(10076, 'Elena Nicholls',      'Dealer', 'Team Kilo', 'Salon Dealer',        '555-2076', 'elena.nicholls@uasm.org',      'S',  'Blackjack, Roulette, Baccarat, Poker',                   '2023-04-19'),
(10077, 'Jordan Grant',        'Dealer', 'Team Kilo', 'Salon Dealer',        '555-2077', 'jordan.grant@uasm.org',        'XL', 'Roulette, Baccarat, Poker, Craps',                       '2023-05-23'),

-- Team Lima (7 members - Optimal 7/7 -> 30-Min Rotation; Poker, Craps, Baccarat, Roulette)
(10078, 'Xavier Rose',         'Dealer', 'Team Lima', 'Senior Pit Dealer',   '555-2078', 'xavier.rose@uasm.org',         'L',  'Blackjack, Roulette, Baccarat, Poker, Craps',            '2023-01-27'),
(10079, 'Naomi Stone',         'Dealer', 'Team Lima', 'Senior Dealer',       '555-2079', 'naomi.stone@uasm.org',         'M',  'Blackjack, Roulette, Baccarat, Poker, Craps',            '2023-02-17'),
(10080, 'Jose Hawkins',        'Dealer', 'Team Lima', 'Specialty Dealer',    '555-2080', 'jose.hawkins@uasm.org',        'M',  'Blackjack, Roulette, Baccarat, Poker, Craps',            '2023-03-23'),
(10081, 'Ruby Dunn',           'Dealer', 'Team Lima', 'Specialty Dealer',    '555-2081', 'ruby.dunn@uasm.org',           'S',  'Blackjack, Roulette, Baccarat, Poker, Craps',            '2023-04-27'),
(10082, 'Colton Perkins',      'Dealer', 'Team Lima', 'Specialty Dealer',    '555-2082', 'colton.perkins@uasm.org',      'XL', 'Roulette, Baccarat, Poker, Craps',                       '2023-05-16'),
(10083, 'Alice Hudson',        'Dealer', 'Team Lima', 'Specialty Dealer',    '555-2083', 'alice.hudson@uasm.org',        'S',  'Roulette, Baccarat, Poker, Craps',                       '2023-06-20'),
(10084, 'Landon Spencer',      'Dealer', 'Team Lima', 'Specialty Dealer',    '555-2084', 'landon.spencer@uasm.org',      'L',  'Blackjack, Roulette, Baccarat, Poker, Craps',            '2023-07-25'),

-- Team Mike (9 members - Overstaffed 9/7; 2 overflow members ready to relocate on Auto-Balance)
(10085, 'Adrian Gardner',      'Dealer', 'Team Mike', 'Senior Pit Dealer',   '555-2085', 'adrian.gardner@uasm.org',      'L',  'Blackjack, Roulette, Baccarat, Poker, Craps',            '2023-01-29'),
(10086, 'Sadie Stephens',      'Dealer', 'Team Mike', 'Senior Dealer',       '555-2086', 'sadie.stephens@uasm.org',      'M',  'Blackjack, Roulette, Baccarat, Poker, Craps',            '2023-02-19'),
(10087, 'Jonathan Payne',      'Dealer', 'Team Mike', 'Multi-Game Dealer',   '555-2087', 'jonathan.payne@uasm.org',      'M',  'Blackjack, Roulette, Baccarat, Poker, Craps',            '2023-03-25'),
(10088, 'Autumn Pierce',       'Dealer', 'Team Mike', 'Multi-Game Dealer',   '555-2088', 'autumn.pierce@uasm.org',       'S',  'Blackjack, Roulette, Poker, Craps',                      '2023-04-29'),
(10089, 'Nolan Berry',         'Dealer', 'Team Mike', 'Multi-Game Dealer',   '555-2089', 'nolan.berry@uasm.org',         'XL', 'Blackjack, Roulette, Poker, Craps',                      '2023-05-21'),
(10090, 'Quinn Matthews',      'Dealer', 'Team Mike', 'Multi-Game Dealer',   '555-2090', 'quinn.matthews@uasm.org',      'S',  'Blackjack, Roulette, Poker, Craps',                      '2023-06-26'),
(10091, 'Everly Reyes',        'Dealer', 'Team Mike', 'Multi-Game Dealer',   '555-2091', 'everly.reyes@uasm.org',        'S',  'Blackjack, Roulette, Baccarat, Poker, Craps',            '2023-07-12'),
(10092, 'Asher Burns',         'Dealer', 'Team Mike', 'Dice & Card Dealer',  '555-2092', 'asher.burns@uasm.org',         'L',  'Blackjack, Roulette, Baccarat, Poker, Craps',            '2023-07-18'),
(10093, 'Parker Arnold',       'Both',   'Team Mike', 'Dual Floor Dealer',   '555-2093', 'parker.arnold@uasm.org',       'L',  'Blackjack, Roulette, Baccarat, Poker, Craps, Game Show', '2023-07-31'),

-- Studio Live (6 members - Understaffed 6/7; 4 Basic Game Show Hosts + 2 Full Dual-Role)
(10094, 'Marcus Thornton',     'Game Show Host', 'Studio Live', 'Lead Show Host', '555-2094', 'marcus.thornton@uasm.org', 'L',  'Game Show',                                              '2023-01-15'),
(10095, 'Vanessa Sterling',    'Game Show Host', 'Studio Live', 'Game Show Host', '555-2095', 'vanessa.sterling@uasm.org','S',  'Game Show',                                              '2023-02-12'),
(10096, 'Damian Cross',        'Game Show Host', 'Studio Live', 'Game Show Host', '555-2096', 'damian.cross@uasm.org',    'M',  'Game Show',                                              '2023-03-18'),
(10097, 'Serena Jenkins',      'Game Show Host', 'Studio Live', 'Game Show Host', '555-2097', 'serena.jenkins@uasm.org',  'M',  'Game Show',                                              '2023-04-22'),
(10098, 'Julian Vance',        'Both',           'Studio Live', 'Dual Show Host', '555-2098', 'julian.vance@uasm.org',    'XL', 'Blackjack, Roulette, Baccarat, Poker, Craps, Game Show', '2023-05-27'),
(10099, 'Jack Mason',          'Both',           'Studio Live', 'Dual Show Host', '555-2099', 'jack.mason@uasm.org',      'L',  'Blackjack, Roulette, Baccarat, Poker, Craps, Game Show', '2023-06-11'),

-- Studio Royale (6 members - Understaffed 6/7; 4 Basic Game Show Hosts + 2 Full Dual-Role)
(10100, 'Sophia Montgomery',   'Game Show Host', 'Studio Royale', 'VIP Show Host',  '555-2100', 'sophia.montgomery@uasm.org', 'S',  'Game Show',                                              '2023-01-21'),
(10101, 'Harrison Blake',      'Game Show Host', 'Studio Royale', 'Game Show Host', '555-2101', 'harrison.blake@uasm.org',    'L',  'Game Show',                                              '2023-02-25'),
(10102, 'Gabriella Mercer',    'Game Show Host', 'Studio Royale', 'Game Show Host', '555-2102', 'gabriella.mercer@uasm.org',  'M',  'Game Show',                                              '2023-03-31'),
(10103, 'Tristan Keller',      'Game Show Host', 'Studio Royale', 'Game Show Host', '555-2103', 'tristan.keller@uasm.org',    'M',  'Game Show',                                              '2023-04-15'),
(10104, 'Camila Delgado',      'Both',           'Studio Royale', 'Dual Show Host', '555-2104', 'camila.delgado@uasm.org',    'S',  'Blackjack, Roulette, Baccarat, Poker, Craps, Game Show', '2023-05-19'),
(10105, 'Maya Knight',         'Both',           'Studio Royale', 'Dual Show Host', '555-2105', 'maya.knight@uasm.org',       'S',  'Blackjack, Roulette, Baccarat, Poker, Craps, Game Show', '2023-06-27');

-- ============================================================================
-- 4. UNIFORM ISSUES (Automatically issue 1 matching team uniform to each employee)
-- ============================================================================
INSERT INTO uniform_issues (employee_id, uniform_id, quantity, issue_date, return_date, status, notes)
SELECT
    e.id,
    u.id,
    1,
    e.join_date,
    '',
    'Issued',
    'Initial team uniform issue'
FROM employees e
JOIN uniforms u ON u.team = e.department AND u.size = e.uniform_size;

-- ============================================================================
-- 5. SAMPLE REASSIGNMENT LOGS
-- ============================================================================
INSERT INTO reassignments (employee_id, from_team, to_team, uniform_id, reassignment_date, notes) VALUES
(10008, 'Team Charlie', 'Team Alpha',  3,  '2026-10-08 09:00:00', 'Assigned to dedicated Blackjack pit'),
(10099, 'Team Hotel',   'Studio Live', 55, '2026-10-09 14:30:00', 'Assigned dual-role dealer to Studio Live');
