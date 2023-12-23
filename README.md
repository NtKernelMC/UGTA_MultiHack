# Халявный DLL Мультичит для проекта Ukraine GTA
- Инжектить любым бесплатным DLL-инжектором еще до подключения на сервер.
- Если уже появилась табличка подключения, просто отключитесь и только тогда инжектите DLL!

# Кнопки

- DELETE включить/отключить убийство с одного выстрела
- END включить/отключить двойной урон на каждый выстрел
- HOME выдать полное здоровье
- F10 выдать бронежелет
- F5 отключить/включить разброс пуль

# Сборка проекта
- Качаем IDE Visual Studio 2022 Community
- Ссылка: https://visualstudio.microsoft.com/ru/vs/community/
- Во время установки, ставим галочку "Разработка класических приложений на С++"
- По окончанию установки, после открытия Visual Studio, нажмите "Клонирование репозитория"
- Вставьте туда ссылку: https://github.com/NtKernelMC/UGTA_MultiHack.git и нажмите "Клонировать"
- ![image](https://github.com/NtKernelMC/UGTA_MultiHack/assets/50884710/ac3f857c-6c24-4dd1-9c43-70a4e1d6538e)
- Меняем тут конфигурацию на Release x86
- Теперь кликаем ПКМ по решени MultiHack и нажимаем "Собрать"
- ![image](https://github.com/NtKernelMC/UGTA_MultiHack/assets/50884710/707277ca-0032-4ffd-ba90-b78a56a74e9a)
- Если проект собрался без ошибок, то увидите подобный вывод в консоль внизу "Успешно выполнено - 1"
- ![image](https://github.com/NtKernelMC/UGTA_MultiHack/assets/50884710/f32551cd-0e7a-437c-b039-fd69aa3b0c48)
- Смотрим путь куда сбилдилась наша дллка, у меня это: C:\Users\RAID\Source\Repos\UGTA_MultiHack\MultiHack\Release\MultiHack.dll
- Вуаля! Вы собрали свой первый чит для UkraineGTA. Копируем .dll`ку в любую папку откуда будет удобно инжектить.
- Инжектим DLL любым паблик инжектором, например загуглите Extreme Injector: https://github.com/master131/ExtremeInjector/releases/tag/v3.7.3
