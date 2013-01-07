#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 10000
#define HASH_SIZE 5000

typedef struct _document
{
    int id;
    char *body;
    struct _document *next;
} DOC;

typedef struct _index
{
    int id;
    struct _index *next;
} INDEX;

typedef struct _indices
{
    INDEX list;
    char *word;
    struct _indices *next;
} INDICES;

INDICES hash_table[HASH_SIZE] = {0};

void add_doc(DOC *head, char *str)
{
    DOC *n;
    static int i = 0;
    
    n = (DOC *)malloc(sizeof(DOC));
    n->id = i++;
    n->body = (char *)malloc(sizeof(char) * (strlen(str) + 1));
    strcpy(n->body, str);
    
    n->next = head->next;
    head->next = n;
}

void read_file(DOC *head, char *file_name)
{
    FILE *fp;
    char line[BUFFER_SIZE];
    
    if ( (fp = fopen(file_name, "r")) == NULL )
    {
        printf("Failed to open: %s\n", file_name);
        exit(1);
    }
    
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        line[strlen(line) - 1] = '\0';  // 改行文字の削除
        add_doc(head, line);
    }
    fclose(fp);
}

void show_list(DOC *head)
{
    DOC *p;
    
    for (p = head->next; p != NULL; p = p->next)
    {
        printf("[%d]: %s\n", p->id, p->body);
    }
}

unsigned calc_hash(char *str)
{
    unsigned int sum;
    for (sum = 0; *str != '\0'; str++)
    {
        sum = (sum * 31) + *str;
    }
    
    return sum % HASH_SIZE;
}


void add_index(INDEX *head, int id)
{
    INDEX *n;
    
    n = (INDEX *)malloc(sizeof(INDEX));
    n->id = id;
    
    n->next = head->next;
    head->next = n;
}

INDEX *get_indices(DOC *head, char *str)
{
    INDEX list;
    DOC *p;
    list.next = NULL;
    for (p = head->next; p != NULL; p = p->next)
    {
        if (strstr(p->body, str) != NULL)
        {
            add_index(&list, p->id);
        }
    }
    
    return list.next;
}


/* テーブルへ追加 */
void add_to_table(DOC *head, char *str)
{
    INDICES *p, *q, *new_ele;
    unsigned hash_val;
    
    hash_val = calc_hash(str);  // ハッシュ値の計算
    p = hash_table[hash_val].next;
    q = &hash_table[hash_val];
    
    // 同じ物があるかどうかチェックしながら検索
    while (p != NULL)
    {
        if (strcmp(p->word, str) == 0)
        {
            break;
        }
        q = p;
        p = p->next;
    }
    
    // ヒットしなかったら追加
    if (p == NULL)
    {
        // 新しく構造体を確保
        new_ele = (INDICES *)malloc(sizeof(INDICES));
        if (new_ele == NULL)
        {
            printf("Memory Allocate Error\n");
            exit(1);
        }
        
        // 単語の領域を確保
        new_ele->word = (char *)malloc(sizeof(char) * (strlen(str) + 1));
        if (new_ele->word == NULL)
        {
            printf("Memory Allocate Error\n");
            exit(1);
        }
        strcpy(new_ele->word, str); // 単語の代入
        (new_ele->list).next = get_indices(head, str);   // 転置インデックスを作る
        q->next = new_ele;          // 末尾に追加
        new_ele->next = NULL;
    }
}

void make_indices(DOC *head)
{
    DOC *p;
    char *body;
    char *word;
    
    for (p = head->next; p != NULL; p = p->next)
    {
        body = (char *)malloc(sizeof(p->body));
        strcpy(body, p->body);
        
        word = strtok(body, " ");
        while (word != NULL)
        {
            add_to_table(head, word);
            word = strtok(NULL, " ");
        }
    }
}

INDEX *and_ope(INDEX *res, INDEX obj)
{
    INDEX *p, *q, *r;
    
    for (p = res->next, q = res; p != NULL; q = p, p = p->next)
    {
        r = obj.next;
        while (r != NULL)
        {
            if (p->id == r->id)
            {
                break;
            }
            r = r->next;
        }
        
        if (r == NULL)
        {
            q->next = p->next;
            free(p);
            p = res->next;  // 設定しなおし
        }
    }
    
    return res;
}


INDEX *search(char *str)
{
    INDICES *p, *q;
    unsigned hash_val;
    char *word;
    INDEX *all_index;
    int first = 1;
    
    
    word = strtok(str, " \n");
    while (word != NULL)
    {
        hash_val = calc_hash(word);  // ハッシュ値の計算
        p = hash_table[hash_val].next;
        q = &hash_table[hash_val];
        
        // ヒットするまで検索
        while (p != NULL)
        {
            if (strcmp(p->word, word) == 0)
            {
                break;
            }
            q = p;
            p = p->next;
        }
        
        if (p == NULL)
        {
            return NULL;
        }
        
        if (first)
        {
            all_index = (p->list).next;
            first = 0;
        }
        else
        {
            and_ope(all_index, p->list);
        }
        
        word = strtok(NULL, " \n");
    }
    
    return all_index;
}

void print_line(INDEX *index, DOC *head)
{
    INDEX *p;
    DOC *q;
    
    for (p = index; p != NULL; p = p->next)
    {
        for (q = head->next; q != NULL && q->id != p->id ; q = q->next)
        {
        }
        if (q != NULL)
        {
            printf("[%d]:%s\n", q->id, q->body);
        }
        else
        {
            printf("err: p->id[%d]\n", p->id);
        }
    }
}

int main(void)
{
    DOC head;
    char *file_name = "news.txt";
    char input[BUFFER_SIZE];
    INDEX *res;
    
    head.next = NULL;
    
    read_file(&head, file_name);
    
    make_indices(&head);
    
    printf("Querry:");
    fgets(input, BUFFER_SIZE, stdin);
    
    res = search(input);
    
    if (res == NULL)
    {
        printf("NOT FOUND\n");
    }
    else
    {
        print_line(res, &head);
    }
    
    
    return 0;
}