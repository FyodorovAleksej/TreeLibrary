#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
	Переопределение слов TRUE и FALSE как 1 и 0 соответсвенно (так как в языке C нету типа bool)
*/
#define TRUE 1
#define FALSE 0

#define MEN 1
#define WOMEN 0


/**
	структура, которая будет храниться в дереве
*/
struct human
{
	char* name;
	char* dateOfBirth;
	int age;
	float payment;
	char sex;
};

/**
	@brief createHuman - создание структуры для хранения в дереве
	@param name - имя
	@param dateOfBirth - дата рождения
	@param age - возраст
	@param payment - зарплата
	@param sex - пол
	@return - указатель на созданную структуру для хранения
*/
struct human* createHuman(char* name, char* dateOfBirth, int age, float payment, char sex)
{
	struct human* result = (struct human*)malloc(sizeof(struct human));	// Выделение памяти под структуру

	int length = 0;		
	length = strlen(name);
	result->name = (char*)malloc(length + 1);							// Выделение памяти под имя
	result->name = strcpy(result->name, name);							// Копирование имени
	result->name[length] = '\0';										// Обрезка имени


	length = strlen(dateOfBirth);										
	result->dateOfBirth = (char*)malloc(length + 1);					// Выделение памяти под дату рождения
	result->dateOfBirth = strcpy(result->dateOfBirth, dateOfBirth);		// Копирование даты рождения
	result->dateOfBirth[length] = '\0';									// Обрезка даты рождения

	result->age = age;
	result->payment = payment;
	result->sex = sex;

	return result;
}

/**
	@brief clearHuman - удаление структуры
	@param human - указатель на структуру, которую нужно удалить
*/
void clearHuman(struct human* human)
{
	if (human != NULL)
	{
		free(human->name);			// Удаление имени
		free(human->dateOfBirth);	// Удаление даты рождения

		free(human);				// Удаление самой структуры
	}
}

/**
	@brief outHuman - вывод структуры в консоль
	@param human - указатель на структуру
	@param level - уровень в дереве
*/
void outHuman(struct human* human, int level)
{
	if (human != NULL)
	{
		for (int i = 0; i < level; i++)	// Отступ
		{
			printf("      ");
		}
		puts(human->name);				// Вывод имени

		for (int i = 0; i < level; i++)
		{
			printf("      ");
		}
		puts(human->dateOfBirth);

		for (int i = 0; i < level; i++)
		{
			printf("      ");
		}
		printf("%d\n", human->age);

		for (int i = 0; i < level; i++)
		{
			printf("      ");
		}
		printf("%f\n", human->payment);

		for (int i = 0; i < level; i++)
		{
			printf("      ");
		}
		if (human->sex == MEN)
		{
			printf("men\n");
		}
		if (human->sex == WOMEN)
		{
			printf("women\n");
		}
	}
}

/**
	@brief humanCompare - сравнение двух структур
	@param human1 - 1-ая структура
	@param human2 - 2-ая структура
	@return -	1  - Если 1-ая структура больше второй
				0  - Если 2-ая структура меньше второй
				-1 - Если они одинаковые
*/
char humanCompare(struct human* human1, struct human* human2)
{
	if (human1 == NULL || human2 == NULL)
	{
		return 1;
	}
	char result = 0;
	result = (char)strcmp(human1->name, human2->name);					// Сравнение имени
	if (result != 0)													// Если неравны
	{
		return result;
	}

	result = (char)strcmp(human1->dateOfBirth, human2->dateOfBirth);	// Сравнение дат рождения
	if (result != 0)													// Если неравны
	{
		return result;
	}

	if (human1->age > human2->age)										// Сравнение возраста
	{
		return 1;
	}
	if (human1->age < human2->age)
	{
		return -1;
	}



	if (human1->payment > human2->payment)								// Сравнение зарплаты
	{
		return 1;
	}
	if (human1->payment < human2->payment)
	{
		return -1;
	}

	return 0;
}

/**
	Определение одного блока (ячейки) дерева
*/
struct block
{
	// Значение самого блока
	struct human* value;
	// Указатель на следующий блок
	struct block* right;
	// Указатель на предыдущий блок
	struct block* left;
};

/**
	@brief create - создание блока (ячейки)
	@param value - значение блока (структура)
	@return - указатель на созданный блок дерева
*/
struct block* create(struct human* value)
{
	struct block* res = (struct block*)malloc(sizeof(struct block));	// Выделение памяти под блок
	res->value = value;
	res->right = NULL;
	res->left = NULL;
	return res;
}


/**
	@brief add - добавление элемента в дерево
	@param root - корень дерева
	@param newBlock - новый блок для добавления
	@return - получилось ли добавление?
*/
char add(struct block* root, struct block* newBlock)
{
	if (humanCompare(newBlock->value, root->value) > 0)		// Если больше чем текущая вершина дерева
	{
		if (root->right != NULL)							// Если есть вершина справа
		{
			return add(root->right, newBlock);				// Переход к правому поддереву
		}
		else
		{
			root->right = newBlock;							// Добавление листа
			return TRUE;
		}
	}

	if (humanCompare(newBlock->value, root->value) < 0)		// Если меньше чем текущая вершина дерева
	{
		if (root->left != NULL)								// Если есть аершина слева
		{
			return add(root->left, newBlock);				// Переход к левому поддереву
		}
		else
		{
			root->left = newBlock;							// Добавление листа
			return TRUE;
		}
	}

	return FALSE;
}

/**
	@brief find - поиск элемента в дереве
	@param root - корень дерева
	@param value - искомое значение(структура)
	@return - хранится ли данное значение в дереве?
*/
char find(struct block* root, struct human* value)
{
	if (humanCompare(value, root->value) > 0)		// Если значение больше, чем текущая вершина дерева
	{
		if (root->right != NULL)					// Если есть поддерево справа
		{
			return find(root->right, value);		// Переход к правому поддереву
		}
		else										// Если дошли до конца
		{
			return FALSE;		
		}
	}

	if (humanCompare(value, root->value) < 0)		// Если значение меньше, чем текущая вершина дерева
	{
		if (root->left != NULL)						// Если есть поддерево слева
		{
			return find(root->left, value);			// Переход к поддереву слева
		}
		else										// Если дошли до конца
		{
			return FALSE;
		}
	}
	return TRUE;									// Если значение совпадает
}


/**
	@brief out - вывод дерева
	@param root - корень дерева (поддерева)
	@param level - уровень глубины текущего поддерева
*/
void out(struct block* root, int level)
{
	if (root->right != NULL)			// Если есть поддерево справа
	{
		out(root->right, level + 1);	// Вывод правого поддерева
	}
	outHuman(root->value, level);		// Вывод значения текущей вершины
	printf("\n");
	if (root->left != NULL)				// Если есть поддерево слева
	{
		out(root->left, level + 1);		// Вывод левого поддерева
	}
}

/**
	@brief del - удаление блока из дерева
	@param root - корень дерева
	@param value - значение, которое нужно удалить из дерева
*/
void del(struct block* root, struct human* value)
{
	if (humanCompare(value, root->value) > 0)	// Поиск вершины дерева для удаления
	{
		if (root->right != NULL)
		{
			del(root->right, value);
			return;
		}
		return;
	}

	if (humanCompare(value, root->value) < 0)
	{
		if (root->left != NULL)
		{
			del(root->left, value);
			return;
		}
		return;
	}

	struct block* current = root;				// Найденная вершина для удаления

	if (current->right != NULL)					// Если есть поддерево справа
	{
		current = current->right;				// Сохранение первого блока справа от удаляемого блока
		struct block* previous = root;			// Сохранение предыдущего блока
		while (current->left != NULL)			// Проход до самого крайнего левого блока
		{
			previous = current;					// Сохранение предыдущего блока
			current = current->left;			// Проход к левому поддереву
		}
												// Текущий блок (current) равен самому крайнему левому блоку (справа от удаляемого блока)
		if (current->right != NULL)				// Если у крайнего левого блока есть поддерево справа
		{
			if (previous == root)				// Если блок справа от удаляемого блока не имеет левого поддерева
			{
				clearHuman(root->value);		// Удаление текущего значения удаляемого блока
				root->value = current->value;	// Замена новым значением
				root->right = current->right;	// Установка правого поддерева без блока справа
				free(current);					// Удаление крайнего блока
			}
			else
			{
				previous->left = current->right;// Замена крайнего блока его правым поддеревом
				clearHuman(root->value);		// Удаление текущего значения удаляемого блока
				root->value = current->value;	// Замена значения удаляемого блока значением крайнего блока
				free(current);					// Удаление крайнего блока
			}
		}
		else
		{
			if (previous == root)				// Если справа от удаляемого блока только 1 блок без поддеревьев
			{
				clearHuman(root->value);		// Удаление текущего значения удаляемого блока
				root->value = current->value;	// Замена его значения
				root->right = current->right;	// Зануление указателя на правое поддерево
				free(current);					// Удаление кранего блока
			}
			else		
			{
				previous->left = NULL;			// Разрывание связи крайнего блока с его родителем
				clearHuman(root->value);		// Удаление текущего значения у удаляемого блока
				root->value = current->value;	// Замена его значения
				free(current);					// Удаление крайнего блока
			}
		}
		return;
	}
	else										// Если справа от удаляемого блока нет поддерева
	{
		clearHuman(root->value);				// Удаление текущего значения удаляемого элемента
		if (root->left != NULL)					// Если есть поддерево слева
		{
			root->value = root->left->value;	// Замена значения
			struct block* current = root->left;	// Вершина слева
			root->left = current->left;			// Замена указателей у родителя
			root->right = current->right;
			free(current);						// Удаление крайнего блока
		}
		else									// Если удаляется лист
		{
			root->value = NULL;					// Зануление значения листа
		}
	}
}

/**
	@brief clear - удаление дерева
	@param root - корень дерева
*/
void clear(struct block* root)
{
	if (root->left != NULL)		// Если есть поддерево слева
	{
		clear(root->left);		// Удаление левого поддерева
	}
	if (root->right != NULL)	// Если есть поддерево справа
	{
		clear(root->right);		// Удаление правого поддерева
	}
	clearHuman(root->value);	// Удаление значения вершины
	free(root);					// Удаление вершины
}


void main()
{
	struct block *start;
	start = create(createHuman("Jame\0", "12.04.1990\0", 27, 4.5, MEN));
	add(start, create(createHuman("Alice\0", "08.06.1991\0", 26, 5.6, WOMEN)));
	add(start, create(createHuman("Adam\0", "12.10.1985\0", 32, 4.3, MEN)));
	add(start, create(createHuman("Kate\0", "11.05.1977\0", 40, 4.7, WOMEN)));
	add(start, create(createHuman("John\0", "05.06.1988\0", 29, 5.3, MEN)));
	add(start, create(createHuman("Amanda\0", "02.01.1990\0", 27, 5.1, WOMEN)));


	out(start, 0);

	printf("--------------------------\n");
	struct human* example = createHuman("Alice\0", "08.06.1991\0", 26, 5.6, WOMEN);
	del(start, example);
	clearHuman(example);


	out(start, 0);
	
	clear(start);
	system("pause");
}