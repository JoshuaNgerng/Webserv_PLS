{
    // Refresh courses when the page load
    // Update courses when form is submitted
    const coursesSection = document.querySelector('#courses-section');
    const courseForm = document.querySelector('#course-form');

    loadCourses();

    if (courseForm) {
        courseForm.onsubmit = function(e) {
            e.preventDefault();

            const image = document.querySelector('#course-image').files[0];
            const title = document.querySelector('#course-title').value;
            const subtitle = document.querySelector('#course-subtitle').value;
            const description = document.querySelector('#course-description').value;

            const formData = new FormData();
            formData.append('course-image', image);
            formData.append('course-title', title);
            formData.append('course-subtitle', subtitle);
            formData.append('course-description', description);

            fetch('/cgi-bin/course_upload.cgi', {
                method: 'POST',
                body: formData
            })
            .then(response => response.json())
            .then(data => {
                if (data.success)
                    loadCourses();
                else
                    console.log('Failed to add course:', data.error);
            })
            .catch(error => console.log('Error:', error));
        }
    }

    function loadCourses() {
        fetch('/cgi-bin/load_courses.cgi')
        .then(response => response.json())
        .then(courses => {
            coursesSection.innerHTML = '';
            courses.forEach(course => {
                const courseElem = document.createElement('article');
                courseElem.setAttribute('data-id', course.Title);
                courseElem.innerHTML = `
                    <div class="delete-btn">_</div>
                    <figure>
                        <img src="${course.Image}" alt="${course.Title}">
                        <figcaption>${course.Title}</figcaption>
                    </figure>
                    <hgroup>
                        <h2>${course.Title}</h2>
                        <h3>${course.Subtitle}</h3>
                    </hgroup>
                    <p>${course.Description}</p>
                `;


                coursesSection.appendChild(courseElem);


                {
                    // Delete course if remove button is clicked
                    const deleteBtn = document.querySelectorAll('.delete-btn');

                    deleteBtn.forEach(btn => {

                        btn.onclick = function() {
                            const courseElem = btn.closest('article');
                            const courseID = courseElem.getAttribute('data-id');

                            fetch(`/cgi-bin/delete_course.cgi?course-title=${courseID}`, {
                                method: 'DELETE'
                            })
                            .then(response => response.json())
                            .then(data => {
                                if (data.success)
                                    courseElem.remove();
                                else
                                    console.log('Failed to delete course:', data.error);
                            })
                            .catch(error => console.log('Error:', error));
                        };
                    })
                }
            })
        })
        .catch(error => console.log('Error loading courses:', error));
    }
}


{
    // Search query functionality (title and subtitle)
    const searchForm = document.querySelector('#search-form');
    const searchBar = document.querySelector('#search-bar');

    searchForm.onsubmit = function(e) {
        e.preventDefault();
        const searchQuery = searchBar.value.toLowerCase();

        fetch(`/cgi-bin/search_courses.cgi?query=${encodeURIComponent(searchQuery)}`)
        .then(response => response.json())
        .then(courses => {
            filterArticles(searchQuery);
        })
        .catch(error => console.log("Error fetching courses:", error));
    };

    function filterArticles(query) {
        console.log(query)
        const articles = document.querySelectorAll('article');

        if (query.length == 0) {
            articles.forEach(article => {
                article.style.display = 'block';
                article.style.visibility = 'visible';
            })
        } else {
            articles.forEach(article => {
                const title = article.querySelector('h2').textContent.toLowerCase();
                const subtitle = article.querySelector('h3').textContent.toLowerCase();

                if (title.includes(query) || subtitle.includes(query))
                    article.style.display = 'block';
                else
                    article.style.display = 'none';
            })


            const visibleArticles = Array.from(articles).filter(article => article.style.display !== 'none');

            if (visibleArticles.length === 0) {
                articles.forEach(article => {
                    article.style.display = 'block';
                    article.style.visibility = 'hidden';
                })
            } else {
                articles.forEach(article => {
                    article.style.visibility = 'visible';
                })
            }

        }
    }
}

{
    // Select the home link or error links
    const homeLink = document.querySelector('#home-link');
    const errorLinks = document.querySelectorAll('.error-link');

    homeLink.onclick = function(e) {
        // e.preventDefault();

        homeLink.classList.add('current');
        errorLinks.forEach(link => link.classList.remove('current'));
    };

    errorLinks.forEach(link => {
        link.onclick = function(e) {
            homeLink.classList.remove('current');
        };
    })
}