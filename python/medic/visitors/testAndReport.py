from . import testOnly


class TestAndReport(testOnly.TestOnly):
    def __init__(self):
        super(TestAndReport, self).__init__()

    def Report(self):
        results = self.results()

        reports = {}
        for tester, res in results.iteritems():
            reports[tester] = []

            for node, comp in res:
                reports[tester].append(node.name())

        return reports