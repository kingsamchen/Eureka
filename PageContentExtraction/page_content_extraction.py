# -*- coding: utf-8 -*-
# @ 0xCCCCCCCC

import logging
import re
import requests

from bs4 import BeautifulSoup

logging.basicConfig(format='%(asctime)s %(message)s', datefmt='%m/%d/%Y %I:%M:%S %p',
                    filename='debug.log', level=logging.DEBUG)


class ExtractionPattern:
    UNLIKELY_CANDIDATE = re.compile('comment|disqus|foot|header|menu|meta|nav|rss|shoutbox|sidebar|sponsor|widget',
                                    re.IGNORECASE)
    POSSIBLE_CANDIDATE = re.compile('article|body|column|main',
                                    re.IGNORECASE)


class PageExtractor:
    def __init__(self, page_in_html):
        soup = BeautifulSoup(page_in_html, 'html.parser')
        self._title = soup.title
        self._document = PageExtractor._sanitize(soup)

        # If the page doesn't have a body tag, we create one at hand.
        if not self._document.find('body'):
            self._document.html.append(soup.new_tag('body'))

    @staticmethod
    def _sanitize(soup):
        """
        Scrapes elements like javascript, css etc.
        """

        for script_ele in soup.find_all('script'):
            script_ele.decompose()

        for style_tag in soup.find_all('style'):
            style_tag.decompose()

        css_link_tags = [link_tag for link_tag in soup.html.find_all('link')
                         if 'stylesheet' in link_tag['rel']]
        for css_link_tag in css_link_tags:
            css_link_tag.decompose()

        return soup

    @staticmethod
    def should_eliminate_tag(tag):
        if tag.name == 'body':
            return False
        class_info = '' if not tag.get('class') else ''.join(tag['class'])
        id_info = '' if not tag.get('id') else tag['id']
        synthesized_info = class_info + ' ' + id_info
        unlikely_hit = ExtractionPattern.UNLIKELY_CANDIDATE.search(synthesized_info) is not None
        possible_hit = ExtractionPattern.POSSIBLE_CANDIDATE.search(synthesized_info) is not None
        return unlikely_hit and not possible_hit

    def scrape_redundancy(self, remove_unlikely_candidates=True):
        soup = self._document
        for tag in reversed(soup.find_all(True)):
            if remove_unlikely_candidates:
                if PageExtractor.should_eliminate_tag(tag):
                    tag.decompose()
                    continue

    @property
    def title(self):
        return self._title

    @property
    def content(self):
        return self._document


def main():
    url = 'http://kingsamchen.github.io/2016/11/19/recommend-csapp-3e-course-video-and-bomb-lab/'
    results = requests.get(url)
    extractor = PageExtractor(results.text)
    extractor.scrape_redundancy()
    print(extractor.content)

if __name__ == '__main__':
    main()
