# Изучаем OpenGL

----

## Как подключить библиотеки с GIT в нашу ветку

- добавляем библиотеку(*в нашем случае GLM*) к нашему локальному репозиторию(**remote add**), указываем название удобное для нас (**glm**) и даем ссылку на GIT:

        git remote add glm https://github.com/g-truc/glm.git

- загружаем subtree в папку external/glm (**--prefix=путь/на/ваш/вкус**), далее указываем наш remote server (**glm**), далее указываем ветку в которую будем делать merge (**master**), далее указываем чтобы все коммиты добавленной ветки были слиты в один коммит (**--squash**)

        git subtree add --prefix=external/glm glm master --squash

- если все хорошо, то мы увидим нечто в роде:

        git fetch glm master
        remote: Enumerating objects: 56346, done.
        remote: Counting objects: 100% (435/435), done.
        remote: Compressing objects: 100% (179/179), done.
        remote: Total 56346 (delta 273), reused 329 (delta 256), pack-reused 55911
        Receiving objects: 100% (56346/56346), 69.09 MiB | 15.60 MiB/s, done.
        Resolving deltas: 100% (42512/42512), done.
        From https://github.com/g-truc/glm
        * branch                master     -> FETCH_HEAD
        * [new branch]          master     -> glm/master
        Added dir 'external/glm'

- теперь добавляем библиотеку в проект, открываем **CMakeLists.txt**:

        add_subdirectory(external/glm)
        target_link_libraries(${PROJECT_NAME} glm)

- готово!
