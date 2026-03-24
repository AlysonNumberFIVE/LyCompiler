#include "../analyser/analyser.h"

static void	free_datatype_list(t_datatype *datatype)
{
	t_datatype	*next;

	while (datatype)
	{
		next = datatype->next;
		free(datatype->type_name);
		free(datatype);
		datatype = next;
	}
}

static void	free_parameter_registers(t_parameter_register *parameters)
{
	t_parameter_register	*next;

	while (parameters)
	{
		next = parameters->next;
		free(parameters->name);
		free_datatype_list(parameters->type);
		free(parameters);
		parameters = next;
	}
}

static t_datatype	*new_datatype(char *type_name, int pointer_level)
{
	t_datatype	*datatype;

	if (type_name == NULL)
		return (NULL);

	datatype = (t_datatype *)malloc(sizeof(t_datatype));
	if (datatype == NULL)
		return (NULL);
        
	datatype->type_name = strdup(type_name);
	if (datatype->type_name == NULL)
	{
		free(datatype);
		return (NULL);
	}

	datatype->pointer_level = pointer_level;
	datatype->is_const = false;
	datatype->struct_def = NULL;
	datatype->next = NULL;
	return (datatype);
}

static t_parameter_register	*append_parameter_register(
	t_parameter_register **head,
	char *name,
	t_datatype *type)
{
	t_parameter_register	*new_register;
	t_parameter_register	*traverse;

	if (head == NULL || name == NULL || type == NULL)
		return (NULL);

	new_register = (t_parameter_register *)malloc(sizeof(t_parameter_register));
	if (new_register == NULL)
		return (NULL);

	new_register->name = strdup(name);
	if (new_register->name == NULL)
	{
		free(new_register);
		return (NULL);
	}

	new_register->type = type;
	new_register->next = NULL;
	if (*head == NULL)
	{
		*head = new_register;
		return (new_register);
	}

	traverse = *head;
	while (traverse->next)
		traverse = traverse->next;
	traverse->next = new_register;
	return (new_register);
}

static int	count_parameters(t_parameter_register *parameters)
{
	int	count;

	count = 0;
	while (parameters)
	{
		count++;
		parameters = parameters->next;
	}
	return (count);
}

t_function_register	*push_function_register(
	t_function_register **head,
	char *name,
	t_datatype *return_type,
	t_parameter_register *parameters,
	t_node *declaration)
{
	t_function_register	*new_register;
	t_function_register	*traverse;

	if (head == NULL || name == NULL || return_type == NULL)
		return (NULL);

	new_register = (t_function_register *)malloc(sizeof(t_function_register));
	if (new_register == NULL)
		return (NULL);

	new_register->name = strdup(name);
	if (new_register->name == NULL)
	{
		free(new_register);
		return (NULL);
	}
	new_register->return_type = return_type;
	new_register->parameters = parameters;
	new_register->parameter_count = count_parameters(parameters);
	new_register->declaration = declaration;
	new_register->next = NULL;
	if (*head == NULL)
	{
		*head = new_register;
		return (new_register);
        
	}
	traverse = *head;
	while (traverse->next)
		traverse = traverse->next;
	traverse->next = new_register;
	return (new_register);
}

t_struct_register	*push_struct_register(
	t_struct_register **head,
	char *name,
	t_node *declaration)
{
	t_struct_register	*new_register;
	t_struct_register	*traverse;

	if (head == NULL || name == NULL)
		return (NULL);
    
	new_register = (t_struct_register *)malloc(sizeof(t_struct_register));
	if (new_register == NULL)
		return (NULL);
    
	new_register->name = strdup(name);
	if (new_register->name == NULL)
	{
		free(new_register);
		return (NULL);
	}
	new_register->declaration = declaration;
	new_register->next = NULL;
	if (*head == NULL)
	{
		*head = new_register;
		return (new_register);
	}

	traverse = *head;
	while (traverse->next)
		traverse = traverse->next;
	traverse->next = new_register;
	return (new_register);
}

t_datatype	*datatype_from_type_node(t_node *type_node)
{
	if (type_node == NULL || type_node->type != NODE_TYPE_SPEC)
		return (NULL);

	return new_datatype(type_node->data.type_spec.base_type,
		type_node->data.type_spec.pointer_level
    );
}

t_parameter_register	*parameter_registers_from_ast(t_node *parameters)
{
	t_parameter_register	*head;
	t_datatype			    *type;

	head = NULL;
	while (parameters)
	{
		if (parameters->type != NODE_PARAM)
		{
			free_parameter_registers(head);
			return (NULL);
		}

		type = datatype_from_type_node(parameters->data.param.type);
		if (type == NULL)
		{
			free_parameter_registers(head);
			return (NULL);
		}

		if (append_parameter_register(&head, parameters->data.param.name, type) == NULL)
		{
			free_datatype_list(type);
			free_parameter_registers(head);
			return (NULL);
		}
		parameters = parameters->next;
	}
	return (head);
}

bool	register_top_level_declarations(
	t_node *program,
	t_function_register **functions,
	t_struct_register **structs)
{
	t_node			        *function_decl;
	t_node			        *struct_decl;
	t_datatype		        *return_type;
	t_parameter_register	*parameters;

	if (program == NULL || functions == NULL || structs == NULL)
		return (false);

	if (program->type != NODE_PROGRAM)
		return (false);

    // Pre-register all structs.
	struct_decl = program->data.program.struct_decl;
	while (struct_decl)
	{
		if (struct_decl->type != NODE_STRUCT_DECL)
			return (false);
        
		if (push_struct_register(structs, struct_decl->data.struct_decl.name, struct_decl) == NULL)
			return (false);
    
		struct_decl = struct_decl->next;
	}
    
	// Pre-register all functions.
	function_decl = program->data.program.function_decl;
	while (function_decl)
	{
		if (function_decl->type != NODE_FUNCTION_DECL)
			return (false);

		return_type = datatype_from_type_node(function_decl->data.func_decl.return_type);
		if (return_type == NULL)
			return (false);
        
		parameters = parameter_registers_from_ast(function_decl->data.func_decl.params);
		if (function_decl->data.func_decl.params != NULL && parameters == NULL)
		{
			free_datatype_list(return_type);
			return (false);
		}

		if (push_function_register(
				functions,
				function_decl->data.func_decl.name,
				return_type,
				parameters,
				function_decl) == NULL)
		{
			free_datatype_list(return_type);
			free_parameter_registers(parameters);
			return (false);
		}
		function_decl = function_decl->next;
	}
	return (true);
}
