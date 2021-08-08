#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/HTMLparser.h>

int Queue_insert(char *url);
void Queue_delete();

int traverse_dom_trees(char *url, xmlNode *a_node)
{
    xmlNode *cur_node = NULL;

    if (NULL == a_node)
    {
        //printf("Invalid argument a_node %p\n", a_node);
        return 0;
    }

    for (cur_node = a_node; cur_node; cur_node = cur_node->next)
    {
        if (cur_node->type == XML_ELEMENT_NODE)
        {
            if (!xmlStrcmp(cur_node->name, (const xmlChar *)"img"))
            {
                xmlChar *src;
                src = xmlGetProp(cur_node, (const xmlChar *)"src");
                char *url_src;
                url_src = malloc(strlen(url) + strlen((char *)src));
                strcpy(url_src, url);
                strcat(url_src, (char *)src);
                if (!Queue_insert(url_src))
                    return 0;
            }

            else if (!xmlStrcmp(cur_node->name, (const xmlChar *)"a"))
            {
                xmlChar *href;
                href = xmlGetProp(cur_node, (const xmlChar *)"href");
                if (!strcmp((const char *)href, "/"))
                {
                    printf("Ignoring /\n");
                }
                else
                {
                    set_url((char *)href);
                    if (!get_url())
                        ;
                    return 0;
                }
            }
        }
        traverse_dom_trees(url, cur_node->children);
    }
    return 1;
}

int searchForImg(char *memory, size_t size, char *url)
{
    htmlDocPtr doc;
    xmlNode *roo_element = NULL;

    /* Macro to check API for match with the DLL we are using */
    LIBXML_TEST_VERSION

    doc = htmlReadMemory(memory, size, url, NULL, HTML_PARSE_NOBLANKS | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING | HTML_PARSE_NONET);
    if (doc == NULL)
    {
        fprintf(stderr, "Document not parsed successfully.\n");
        return 0;
    }

    roo_element = xmlDocGetRootElement(doc);

    if (roo_element == NULL)
    {
        fprintf(stderr, "empty document\n");
        xmlFreeDoc(doc);
    }

    else
    {
        printf("Root Node is %s\n", roo_element->name);
        if (!traverse_dom_trees(url, roo_element))
            return 0;

        xmlFreeDoc(doc);    // free document
        xmlCleanupParser(); // Free globals
    }

    return 1;
}
