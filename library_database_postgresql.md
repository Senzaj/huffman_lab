# Library Database — PostgreSQL

## 1. Create tables

```sql
-- ============================================
-- LIBRARY DATABASE
-- ============================================

-- ============================================
-- PUBLISHERS
-- ============================================

CREATE TABLE publishers (
    publisher_id INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    country VARCHAR(60) NOT NULL,
    phone VARCHAR(20),
    email VARCHAR(100),
    founded_date DATE CHECK (
        founded_date BETWEEN DATE '1400-01-01' AND CURRENT_DATE
    )
);


-- ============================================
-- BOOKS
-- ============================================

CREATE TABLE books (
    book_id INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    title VARCHAR(150) NOT NULL,
    author VARCHAR(100) NOT NULL,
    publication_date DATE CHECK (
        publication_date BETWEEN DATE '1400-01-01' AND CURRENT_DATE
    ),
    rating NUMERIC(2,1) CHECK (
        rating BETWEEN 0.0 AND 10.0
    ),
    pages INTEGER CHECK (pages > 0),
    publisher_id INTEGER NOT NULL,

    CONSTRAINT fk_books_publisher
        FOREIGN KEY (publisher_id)
        REFERENCES publishers(publisher_id)
        ON UPDATE CASCADE
        ON DELETE RESTRICT
);


-- ============================================
-- READERS
-- ============================================

CREATE TABLE readers (
    reader_id INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    last_name VARCHAR(50) NOT NULL,
    first_name VARCHAR(50) NOT NULL,
    middle_name VARCHAR(50),
    birth_date DATE,
    phone VARCHAR(20),
    membership_years INTEGER NOT NULL CHECK (
        membership_years BETWEEN 0 AND 80
    )
);


-- ============================================
-- ISSUANCES
-- ============================================

CREATE TABLE issuances (
    issuance_id INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    reader_id INTEGER NOT NULL,
    book_id INTEGER NOT NULL,
    issued_date DATE NOT NULL,
    due_date DATE NOT NULL,
    returned_date DATE,
    status VARCHAR(20) NOT NULL DEFAULT 'Borrowed',

    CONSTRAINT fk_issuances_reader
        FOREIGN KEY (reader_id)
        REFERENCES readers(reader_id)
        ON UPDATE CASCADE
        ON DELETE RESTRICT,

    CONSTRAINT fk_issuances_book
        FOREIGN KEY (book_id)
        REFERENCES books(book_id)
        ON UPDATE CASCADE
        ON DELETE RESTRICT,

    CONSTRAINT check_issuance_dates
        CHECK (due_date >= issued_date),

    CONSTRAINT check_return_date
        CHECK (
            returned_date IS NULL
            OR returned_date >= issued_date
        ),

    CONSTRAINT check_issuance_status
        CHECK (
            status IN ('Borrowed', 'Returned', 'Overdue')
        )
);
```

## 2. Insert publishers

```sql
INSERT INTO publishers
(name, country, phone, email, founded_date)
VALUES
('Fabledom', 'United Kingdom', '+44-20-7927-3800', 'contact@fabel.co.uk', '1925-04-01'),
('Midnight Home', 'Canada', '+1-416-555-0182', 'hello@midnight.ca', '1978-09-17'),
('Activision', 'Finland', '+358-9-555-0147', 'office@activision.fi', '1964-02-11'),
('Raven & Oak', 'Ireland', '+353-1-555-0236', 'books@ravenandoak.ie', '1987-06-23'),
('Silver Bird Publishing', 'Australia', '+61-2-5550-4172', 'info@silverbirch.au', '2003-11-08');
```

## 3. Insert books

```sql
INSERT INTO books
(title, author, publication_date, rating, pages, publisher_id)
VALUES
('The Shadow of the Wind', 'Carlos Ruiz Zafón', '2001-04-17', 9.1, 487, 1),
('Piranesi', 'Susanna Clarke', '2020-09-15', 4.8, 272, 2),
('The Left Hand of Darkness', 'Ursula K. Le Guin', '1969-03-01', 9.3, 304, 3),
('The Book of Disquiet', 'Fernando Pessoa', '1982-01-01', 8.5, 512, 4),
('A Gentleman in Moscow', 'Amor Towles', '2016-03-08', 5.9, 462, 5),
('Never Let Me Go', 'Kazuo Ishiguro', '2005-03-03', 8.6, 288, 1),
('The Book', 'Unknown', '2025-07-12', 9.9, 712, 1),
('The City & the City', 'China Miéville', '2009-05-27', 7.2, 312, 3);
```

## 4. Insert readers

```sql
INSERT INTO readers
(last_name, first_name, middle_name, birth_date, phone, membership_years)
VALUES
('Morozov', 'Kirill', 'Antonovich', '1998-02-14', '+7-921-483-7162', 6),
('Belova', 'Elena', 'Romanovna', '2001-07-29', '+7-911-274-5038', 3),
('Griffin', 'Megan', NULL, '1997-11-03', '+44-7700-912184', 8),
('Sokolov', 'Artem', 'Denisovich', '2003-05-18', '+7-931-638-2941', 1),
('Walsh', 'Connor', 'James', '1995-09-26', '+353-85-713-4628', 11),
('Oleg', 'Evpatij', NULL, '1891-12-01', '+1-(555)-555-1234', 8),
('Karpova', 'Nina', 'Olegovna', '2000-12-07', '+7-916-825-3740', 4);
```

## 5. Insert issuances

```sql
INSERT INTO issuances
(reader_id, book_id, issued_date, due_date, returned_date, status)
VALUES
(1, 3, '2026-08-12', '2026-08-26', '2026-08-24', 'Returned'),
(1, 6, '2026-09-03', '2026-09-17', NULL, 'Borrowed'),
(2, 2, '2026-08-28', '2026-09-11', '2026-09-09', 'Returned'),
(3, 1, '2026-09-06', '2026-09-20', NULL, 'Borrowed'),
(4, 5, '2026-08-02', '2026-08-16', '2026-08-21', 'Overdue'),
(5, 4, '2026-09-10', '2026-09-24', NULL, 'Borrowed'),
(2, 1, '2026-06-18', '2026-07-02', '2026-06-30', 'Returned'),
(3, 3, '2026-07-14', '2026-07-28', '2026-07-31', 'Overdue'),
(6, 7, '2026-09-14', '2026-09-28', NULL, 'Borrowed'),
(1, 5, '2026-09-16', '2026-09-30', NULL, 'Borrowed');
```

## 6. Select query

### All issuances with reader, book and publisher

```sql
SELECT
    i.issuance_id,
    r.last_name || ' ' || r.first_name AS reader,
    b.title AS book,
    b.author,
    p.name AS publisher,
    i.issued_date,
    i.due_date,
    i.returned_date,
    i.status
FROM issuances i
JOIN readers r
    ON i.reader_id = r.reader_id
JOIN books b
    ON i.book_id = b.book_id
JOIN publishers p
    ON b.publisher_id = p.publisher_id
ORDER BY i.issued_date DESC;
```

### Currently borrowed books

```sql
SELECT
    r.last_name || ' ' || r.first_name AS reader,
    b.title AS book,
    b.author,
    b.rating,
    i.issued_date,
    i.due_date
FROM issuances i
JOIN readers r
    ON i.reader_id = r.reader_id
JOIN books b
    ON i.book_id = b.book_id
WHERE i.status = 'Borrowed'
ORDER BY i.due_date;
```

## 7. Database relationships

```text
publishers (1) ──────── (N) books
                            |
                            |
                           (1)
                            |
                            |
                           (N)
                       issuances
                            |
                            |
                           (N)
                            |
                            |
                           (1)
                         readers
```
