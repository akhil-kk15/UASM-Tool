-- Seed script for AIDS (Uniform & Shift Management System)

-- Recreate clean tables
DROP TABLE IF EXISTS uniform_issues;
DROP TABLE IF EXISTS uniforms;
DROP TABLE IF EXISTS employees;
DROP TABLE IF EXISTS teams;
DROP TABLE IF EXISTS team_schedules;

CREATE TABLE employees (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    department TEXT,
    designation TEXT,
    uniform_size TEXT DEFAULT 'M',
    email TEXT,
    phone TEXT,
    role_type TEXT DEFAULT 'Dealer'
);

CREATE TABLE uniforms (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    type TEXT NOT NULL,
    quantity INTEGER,
    size TEXT,
    color TEXT,
    description TEXT,
    team TEXT DEFAULT ''
);

CREATE TABLE uniform_issues (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    employee_id INTEGER NOT NULL,
    uniform_id INTEGER NOT NULL,
    quantity INTEGER,
    issue_date DATE,
    return_date DATE,
    status TEXT DEFAULT 'Issued',
    FOREIGN KEY(employee_id) REFERENCES employees(id),
    FOREIGN KEY(uniform_id) REFERENCES uniforms(id)
);

CREATE TABLE teams (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT UNIQUE NOT NULL,
    uniform_type TEXT,
    target_staff INTEGER DEFAULT 5,
    min_staff INTEGER DEFAULT 4,
    max_staff INTEGER DEFAULT 7,
    team_type TEXT DEFAULT 'Standard'
);

CREATE TABLE team_schedules (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    team_name TEXT NOT NULL,
    shift_date DATE NOT NULL,
    slot_index INTEGER NOT NULL,
    start_time TEXT,
    end_time TEXT,
    table_assignments TEXT,
    break_employees TEXT
);

-- 1. Insert Teams (Standard & Game Show Host)
INSERT INTO teams (id, name, uniform_type, target_staff, min_staff, max_staff, team_type) VALUES
(1, 'Team Alpha',  'Alpha Vest',  5, 4, 7, 'Standard'),
(2, 'Team Beta',   'Beta Shirt',  5, 4, 7, 'Standard'),
(3, 'Team Gamma',  'Gamma Suit',  5, 4, 7, 'Standard'),
(4, 'Team Delta',  'Delta Polo',  5, 4, 7, 'Standard'),
(5, 'Studio Live', 'Studio Gold', 5, 4, 7, 'Game Show Host');

-- 2. Insert Uniforms in Inventory (Office Stock)
INSERT INTO uniforms (id, type, quantity, size, color, description, team) VALUES
-- Team Alpha Uniforms
(1, 'Alpha Vest', 5, 'M', 'Navy Blue', 'Standard Alpha Vest (Size M)', 'Team Alpha'),
(2, 'Alpha Vest', 4, 'L', 'Navy Blue', 'Standard Alpha Vest (Size L)', 'Team Alpha'),
(3, 'Alpha Vest', 3, 'S', 'Navy Blue', 'Standard Alpha Vest (Size S)', 'Team Alpha'),

-- Team Beta Uniforms (NOTE: Size M is intentionally 0 stock for testing fallback redirection!)
(4, 'Beta Shirt', 0, 'M', 'White / Black Trim', 'Beta Table Shirt (Size M) - OUT OF STOCK', 'Team Beta'),
(5, 'Beta Shirt', 3, 'L', 'White / Black Trim', 'Beta Table Shirt (Size L)', 'Team Beta'),
(6, 'Beta Shirt', 2, 'XL', 'White / Black Trim', 'Beta Table Shirt (Size XL)', 'Team Beta'),

-- Team Gamma Uniforms
(7, 'Gamma Suit', 6, 'M', 'Charcoal Gray', 'Gamma Suit & Vest (Size M)', 'Team Gamma'),
(8, 'Gamma Suit', 4, 'L', 'Charcoal Gray', 'Gamma Suit & Vest (Size L)', 'Team Gamma'),
(9, 'Gamma Suit', 3, 'S', 'Charcoal Gray', 'Gamma Suit & Vest (Size S)', 'Team Gamma'),

-- Team Delta Uniforms
(10, 'Delta Polo', 4, 'M', 'Burgundy', 'Delta Team Polo (Size M)', 'Team Delta'),
(11, 'Delta Polo', 3, 'L', 'Burgundy', 'Delta Team Polo (Size L)', 'Team Delta'),
(12, 'Delta Polo', 2, 'XL', 'Burgundy', 'Delta Team Polo (Size XL)', 'Team Delta'),

-- Studio Live (Game Show Host) Uniforms
(13, 'Studio Gold', 5, 'M', 'Gold & Black', 'Studio Live Host Blazer (Size M)', 'Studio Live'),
(14, 'Studio Gold', 4, 'L', 'Gold & Black', 'Studio Live Host Blazer (Size L)', 'Studio Live'),
(15, 'Studio Gold', 3, 'S', 'Gold & Black', 'Studio Live Host Blazer (Size S)', 'Studio Live');

-- 3. Insert Employees with Roles (Dealer, Game Show Host, Both)
INSERT INTO employees (id, name, department, designation, uniform_size, email, phone, role_type) VALUES
-- Team Alpha: Overstaffed with 7 members (Target 5)
(1, 'Alice Smith',    'Team Alpha', 'Lead Dealer',     'M', 'alice@office.com',   '555-0101', 'Both'),
(2, 'Bob Johnson',    'Team Alpha', 'Senior Dealer',   'L', 'bob@office.com',     '555-0102', 'Dealer'),
(3, 'Charlie Davis',  'Team Alpha', 'Dealer',          'M', 'charlie@office.com', '555-0103', 'Dealer'),
(4, 'Diana Prince',   'Team Alpha', 'Host / Dealer',   'S', 'diana@office.com',   '555-0104', 'Both'),
(5, 'Evan Wright',    'Team Alpha', 'Dealer',          'L', 'evan@office.com',    '555-0105', 'Dealer'),
(6, 'Fiona Gallagher','Team Alpha', 'Relief Dealer',   'M', 'fiona@office.com',   '555-0106', 'Dealer'),
(7, 'George Clark',   'Team Alpha', 'Relief Dealer',   'M', 'george@office.com',  '555-0107', 'Both'),

-- Team Beta: Critically Understaffed with 3 members (Target 5, Min 4)
(8,  'Hannah Abbott', 'Team Beta',  'Dealer',          'L',  'hannah@office.com', '555-0201', 'Dealer'),
(9,  'Ian Malcolm',   'Team Beta',  'Dealer',          'XL', 'ian@office.com',    '555-0202', 'Dealer'),
(10, 'Julia Roberts', 'Team Beta',  'Dealer',          'L',  'julia@office.com',  '555-0203', 'Dealer'),

-- Team Gamma: Optimal Staffing with 5 members (Target 5)
(11, 'Kevin Bacon',   'Team Gamma', 'Dealer',          'M', 'kevin@office.com',   '555-0301', 'Dealer'),
(12, 'Laura Croft',   'Team Gamma', 'Dealer',          'S', 'laura@office.com',   '555-0302', 'Dealer'),
(13, 'Michael Scott', 'Team Gamma', 'Dealer',          'M', 'michael@office.com', '555-0303', 'Dealer'),
(14, 'Nina Williams', 'Team Gamma', 'Dealer',          'L', 'nina@office.com',    '555-0304', 'Dealer'),
(15, 'Oscar Martinez','Team Gamma', 'Dealer',          'M', 'oscar@office.com',   '555-0305', 'Both'),

-- Team Delta: Understaffed with 4 members (Target 5, needs 1 for relief)
(16, 'Peter Parker',   'Team Delta', 'Dealer',         'M',  'peter@office.com',  '555-0401', 'Dealer'),
(17, 'Quinn Fabray',   'Team Delta', 'Dealer',         'L',  'quinn@office.com',  '555-0402', 'Dealer'),
(18, 'Riley Reid',     'Team Delta', 'Dealer',         'S',  'riley@office.com',  '555-0403', 'Both'),
(19, 'Sam Winchester', 'Team Delta', 'Dealer',         'XL', 'sam@office.com',    '555-0404', 'Dealer'),

-- Studio Live: Dedicated Game Show Host Team (5 members)
(20, 'Luna Lovegood',  'Studio Live', 'Game Show Host', 'M', 'luna@office.com',   '555-0501', 'Game Show Host'),
(21, 'Mason Mount',    'Studio Live', 'Game Show Host', 'L', 'mason@office.com',  '555-0502', 'Game Show Host'),
(22, 'Nora Jones',     'Studio Live', 'Host & Dealer',  'S', 'nora@office.com',   '555-0503', 'Both'),
(23, 'Oliver Queen',   'Studio Live', 'Lead Host',      'L', 'oliver@office.com', '555-0504', 'Game Show Host'),
(24, 'Piper Halliwell','Studio Live', 'Game Show Host', 'M', 'piper@office.com',  '555-0505', 'Game Show Host');

-- 4. Insert Uniform Issues
INSERT INTO uniform_issues (id, employee_id, uniform_id, quantity, issue_date, return_date, status) VALUES
(1, 1,   1, 1, '2026-10-01', NULL, 'Issued'),
(2, 2,   2, 1, '2026-10-01', NULL, 'Issued'),
(3, 3,   1, 1, '2026-10-02', NULL, 'Issued'),
(4, 8,   5, 1, '2026-10-01', NULL, 'Issued'),
(5, 11,  7, 1, '2026-10-01', NULL, 'Issued'),
(6, 16, 10, 1, '2026-10-01', NULL, 'Issued'),
(7, 20, 13, 1, '2026-10-01', NULL, 'Issued'),
(8, 2,   1, 1, '2026-09-01', '2026-09-30', 'Returned');
