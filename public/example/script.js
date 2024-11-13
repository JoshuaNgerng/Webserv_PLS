{
    // Refresh courses when the page load
    // Update courses when form is submitted
    const coursesSection = document.querySelector('#courses-section');
    const courseForm = document.querySelector('#course-form');

    loadCourses();

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

        fetch('', {
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

    function loadCourses() {
        fetch('')
        .then(response => response.json())
        .then(courses => {
            coursesSection.innerHTML = '';
            courses.forEach(course => {
                const courseElem = document.createElement('article');
                courseElem.setAttribute('data-id', course.id);
                courseElem.innerHTML = `
                    <div class="delete-btn">_</div>
                    <figure>
                        <img src="${course.image}" alt="${course.title}">
                        <figcaption>${course.title}</figcaption>
                    </figure>
                    <hgroup>
                        <h2>${course.title}</h2>
                        <h3>${course.subtitle}</h3>
                    </hgroup>
                    <p>${course.description}</p>
                `;

                coursesSection.appendChild(courseElem);
            })
        })
        .catch(error => console.log('Error loading courses:', error));
    }
}

{
    // Delete course if remove button is clicked
    const deleteBtn = document.querySelectorAll('.delete-btn');

    deleteBtn.forEach(btn => {
        btn.onclick = function() {
            const courseElem = btn.closest('article');
            const courseID = courseElem.getAttribute('data-id');

            fetch(`${courseID}`, {
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


{
    // Search query functionality (title and subtitle)
    const searchForm = document.querySelector('#search-form');
    const searchBar = document.querySelector('#search-bar');

    searchForm.onsubmit = function(e) {
        e.preventDefault();
        const searchQuery = searchBar.value.toLowerCase();
        filterArticles(searchQuery);
    };

    function filterArticles(query) {
        const articles = document.querySelectorAll('article');

        if (query === '') {
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
    // Hide/Unhide aside sections for home and contact link
    const homeLink = document.querySelector('#home-link');
    const contactLink = document.querySelector('#contact-link');
    const addCourseSection = document.querySelector('#add-course-section');
    const addContactSection = document.querySelector('#add-contact-section');

    homeLink.onclick = function(e) {
        e.preventDefault();

        addCourseSection.style.display = 'block';
        addContactSection.style.display = 'none';

        homeLink.classList.add('current');
        contactLink.classList.remove('current');
    };

    contactLink.onclick = function(e) {
        e.preventDefault();

        addCourseSection.style.display = 'none';
        addContactSection.style.display = 'block';

        homeLink.classList.remove('current');
        contactLink.classList.add('current');
    }
}