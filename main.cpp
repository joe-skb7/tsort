#include <cstdlib>
#include <iostream>
#include <list>

using namespace std;

typedef string vertex;
typedef pair<vertex, vertex> edge;	// ".first" depends on ".second"
typedef list<edge> graph;
typedef list<vertex> vlist;		// list of vertices (result list)

bool ready_to_remove(graph &g, const vertex &v)
{
	for (auto i : g)
		if (i.first == v && i.second != v)
			return false;
	return true;
}

void remove_dependency(graph &g, const vertex &dep)
{
	for (auto i = g.begin(); i != g.end(); ++i)
		if (i->second == dep)
			i->second = i->first;
}

bool handle_standalone_vertices(graph &g, vlist &res)
{
	bool ret = false;

	for (auto i = g.begin(); i != g.end(); ++i) {
		if (i->first == i->second && ready_to_remove(g, i->first)) {
			remove_dependency(g, i->first);
			if (res.back() != i->first)
				res.push_back(i->first);

			i = g.erase(i);
			--i;

			ret = true;
		}
	}

	return ret;
}

bool tsort(const graph &gr, vlist &res)
{
	graph g = gr;

	while (!g.empty())
		if (!handle_standalone_vertices(g, res))
			return false;
	return true;
}

/* -------------------------------------------------------------------------- */

void print_vlist(const vlist &v)
{
	for (auto i : v)
		cout << i << endl;
}

void print_graph(const graph &g)
{
	for (auto i : g)
		cout << i.first << " " << i.second << endl;
}

int main()
{
	graph g;
	vlist v;

	// Populate graph
	g.push_back(make_pair("a", "b"));
	g.push_back(make_pair("a", "x"));
	g.push_back(make_pair("a", "y"));
	g.push_back(make_pair("a", "z"));
	g.push_back(make_pair("b", "c"));
	g.push_back(make_pair("c", "d"));
	g.push_back(make_pair("d", "d"));
	g.push_back(make_pair("x", "y"));
	g.push_back(make_pair("y", "z"));
	g.push_back(make_pair("z", "z"));
	g.push_back(make_pair("m", "a"));
	g.push_back(make_pair("m", "b"));

	if (!tsort(g, v)) {
		cerr << "Error: Circular dependencies found" << endl;
		return EXIT_FAILURE;
	}

	cout << "Result:" << endl;
	print_vlist(v);

	return EXIT_SUCCESS;
}
