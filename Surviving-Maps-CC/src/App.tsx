import { useState, useEffect } from 'react'
import parse from "html-react-parser"

import reactLogo from './assets/react.svg'
import survivingLogo from '/Logo.png'

import appMain from './tscomponents/appMain'
import Modal from 'react-modal';

Modal.setAppElement('#Info');

function App() {
  // Header
  // AppBody:{
  // Search Parameters
  // item(s) View
  // Result view
  // }
  // Footer

  const [modalIsOpen, setIsOpen] = useState(false);
  const [modalPageName, setModalPageName] = useState("");
  const [modalLastFetchName, setModalLastFetchName] = useState("");
  const [modalPageContents, setModalPageContents] = useState("");
  const APIURL: string = "/motd";

  interface FAQContents {
    items?: string[];
    version?: string;
    title?: string;
  }
  interface FAQSection {
    heading?: string;
    subheading?: string;
    contents?: FAQContents[];
  }
  interface FAQResponse {
    sections?: FAQSection[];
  }

  async function fetchModPage(apiURL: string, pageStr: string) {

    if (pageStr == modalLastFetchName) {
      if (pageStr == "") {
        return;
      }
      if (modalPageContents != "") {
        return;
      }
    }

    const response = await fetch(apiURL,
      { method: "POST",
        headers: {"Content-Type": "application/json"},
        body: JSON.stringify({ page: pageStr })
      }
    );
  
    if(response.status == 200) {
      let pageson: FAQResponse = await response.json();
      let contentstr: string = "";

      if (pageson.sections) {
        for (const section of pageson.sections) {
          if (section.heading) {
            contentstr += "<h1>";
            contentstr += section.heading;
            contentstr += "</h1>";
          }
          if (section.subheading) {
            contentstr += "<h2>";
            contentstr += section.subheading;
            contentstr += "</h2>";
          }
          if (section.contents) {
            for (const contitem of section.contents) {
              if (contitem.version) {
                contentstr += "<h3><u>Version: ";
                contentstr += contitem.version;
                contentstr += "</u></h3>";
              }
              if (contitem.title) {
                contentstr += "<h3><u>";
                contentstr += contitem.title;
                contentstr += "</u></h3>";
              }
              if (contitem.items) {
                contentstr += "<ul>";
                for (const item of contitem.items) {
                  contentstr += "<li>";
                  contentstr += item;
                  contentstr += "</li>";
                }
                contentstr += "</ul>";
              }
            }
          }
        }
      }

      setModalPageContents(contentstr);
      setModalLastFetchName(pageStr);
      return;
    }

    if(response.status < 600 || response.status >= 500) {
      // Failed, from server
      setModalPageContents("<h5>Server error</h5");
      setModalLastFetchName("");
      return;
    }

    // Fail through
    setModalPageContents("<h5>Unknown error</h5");
    setModalLastFetchName("");
    return;
  }

  function openModal(page: string) {
    setModalPageName(page);
    fetchModPage(APIURL, page);
    setIsOpen(true);
  }

  function afterOpenModal() {
    
  }

  function closeModal() {
    setIsOpen(false);
    setModalPageName("");
  }

  useEffect(() => {
    if(!modalIsOpen) {
      return;
    }

    fetchModPage(APIURL, modalPageName);
  }, [])

  return [
    <>
    <div id="Header" className="row">
      <nav aria-label="navigationDiv" className="navbar navbar-expand-md navbar-dark bg-dark fixed-top">
        <div className="container-fluid">
          <a href="https://react.dev" target="_blank" className="navbar">
            <img src={reactLogo} className="logo react" alt="React logo" />
          </a>
          <p>/</p>
          <a href="https://survivingmars.paradoxwikis.com/Surviving_Mars_Wiki" className="navbar-brand">
            <img src={survivingLogo} className="logo survivingmars" alt="Surviving Mars Logo" />
          </a>
          <a className="navbar-brand" href="/">Surviving Maps.cc <b>: Original</b></a>
          <button aria-controls="navbarNavDropdown" aria-expanded="false" aria-label="Toggle navigation" className="navbar-toggler"
                    data-bs-target="#navbarNavDropdown" data-bs-toggle="collapse" type="button">
            <span className="navbar-toggler-icon"></span>
          </button>
          <div className="collapse navbar-collapse" id="navbarNavDropdown">
            <ul className="navbar-nav">
              <li className="nav-item" id="Home">
                <a className="nav-link" href="https://original.surviving-maps.cc/">Home</a>
              </li>
              <li>
                <a className="nav-link" href="https://surviving-maps.cc/">[Relaunched]</a>
              </li>
              <li className="nav-item" id="FAQ">
                <a className="nav-link" onClick={() => openModal("FAQ")}>FAQ</a>
              </li>
              <li className="nav-item" id="Changelog">
                <a className="nav-link" onClick={() => openModal("Changelog")}>Changelog</a>
              </li>
            </ul>
            </div>
        </div>
      </nav>
    </div>
    </>,
    <Modal
      isOpen={modalIsOpen}
      onAfterOpen={afterOpenModal}
      onRequestClose={closeModal}
      >
        <button onClick={closeModal}>close</button>
        <div id='ModalContent'>{parse(modalPageContents)}</div>
    </Modal>
    ,appMain(),
    <>
    <div id="Footer">
      <nav aria-label="footerDiv" className="navbar navbar-expand-md navbar-dark bg-dark fixed-bottom">
        <div className="container-fluid row">
          <div className="collapse navbar-collapse" id="navbown">
            <div className="col-8 text-white">
              This website has no guarantees and has been developed as a hobby project
            </div>
          </div>
        </div>
      </nav>
    </div>
    </>
  ];
}

export default App
