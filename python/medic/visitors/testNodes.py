from ..core import visitor
from ..core import nodes


class TestNodes(visitor.Visitor):
    def __init__(self):
        super(TestNodes, self).__init__()

    def visit(self, karte):
        karte.resetResults()

        all_nodes = nodes.GetNodes()

        for tester in karte.testers():
            test_nodes = filter(lambda x : tester.Match(x), all_nodes)

            for node in test_nodes:
                if tester.Test(node):
                    karte.addResult(tester, node)
