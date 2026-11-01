document.querySelectorAll('nav button').forEach(b=>b.addEventListener('click',()=>{
 document.querySelectorAll('nav button').forEach(x=>x.classList.remove('active'));b.classList.add('active');
 document.querySelectorAll('main section').forEach(s=>s.classList.add('hidden'));
 document.getElementById(b.dataset.tab).classList.remove('hidden');
}));
document.getElementById('file').addEventListener('change',e=>{
 const f=e.target.files[0];document.getElementById('fileinfo').textContent=f?`${f.name}\\n${f.size} bytes\\nBrowser preview only; authoritative parsing is performed by the C++ backend.`:'No file selected.';
});
