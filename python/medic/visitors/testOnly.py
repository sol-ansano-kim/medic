from ..core import visitor
from ..core import nodes


class TestOnly(visitor.Visitor):
    def __init__(self):
        super(TestOnly, self).__init__()

    def visit(self, karte):
        self.resetResults()

        all_nodes = nodes.GetNodes()

        for tester in karte.testers():
            test_nodes = filter(lambda x : tester.Match(x), all_nodes)

            for node in test_nodes:
                (result, component) = tester.Test(node)
                if result:
                    self.addResult(tester, (node, component))
