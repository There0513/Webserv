
<?php
if(isset($_POST['last_name'])) {
    echo $_POST['last_name'];
}
?>
<!-- last_name corresponnd au name de l'input dans le formulaire html -->
<form action="post_form.php" method="POST">
    <label>Last Name : </label></br>
    <input class="input" name="last_name" id="name" required>
    <br>
    <button type="submit" id="get_form" class="formbtn">
    <pre class="btn-pre">S U B M I T</pre>
    </button>
</form>
 