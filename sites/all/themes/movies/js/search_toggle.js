function toggle_search_exposed(id) {
  var e = document.getElementById(id);

  // Use for small screen.
  if (e.style.display === 'block') {
    e.style.display = 'none';
  } else {
    e.style.display = 'block';
  }

  // Use for screen larger than 1024px.
  if (e.style.visibility === 'visible') {
    e.style.visibility = 'hidden';
    e.style.opacity = 0;
  }
  else {
    e.style.visibility = 'visible';
    e.style.opacity = 1;
  }
}
