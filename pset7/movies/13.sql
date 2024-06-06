select distinct(name) from people
join stars on people.id = stars.person_id
join movies on stars.movie_id = movies.id
where not name = 'Kevin Bacon'
and movies.id in (

select movies.id from people
join stars on people.id = stars.person_id
join movies on stars.movie_id = movies.id
where name = 'Kevin Bacon')

;